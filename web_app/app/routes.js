// app/routes.js

var util = require('util');
var async = require('async');

var db_query = require('../config/db_queries.js');
//var scheduler = require('../cpp_node_conn/build/Release/scheduler');

module.exports = function(app, passport, formidable, pg) {

    // =====================================
    // HOME PAGE (with login links) ========
    // =====================================
    app.get('/', function(req, res) {
        res.render('index.ejs'); // load the index.ejs file
    });

    // =====================================
    // LOGIN ===============================
    // =====================================
    // show the login form
    app.get('/login', function(req, res) {

        // render the page and pass in any flash data if it exists
        res.render('login.ejs', { message: req.flash('loginMessage') }); 
    });

    // process the login form
    // app.post('/login', do all our passport stuff here);

    // =====================================
    // SIGNUP ==============================
    // =====================================
    // show the signup form
    app.get('/signup', function(req, res) {

        // render the page and pass in any flash data if it exists
        res.render('signup.ejs', { message: req.flash('signupMessage') });
    });

    // process the signup form
    app.post('/signup', passport.authenticate('local-signup', {
        successRedirect : '/profile', // redirect to the secure profile section
        failureRedirect : '/signup', // redirect back to the signup page if there is an error
        failureFlash : true // allow flash messages
    }));

    // process the login form
    app.post('/login', passport.authenticate('local-login', {
        successRedirect : '/profile', // redirect to the secure profile section
        failureRedirect : '/login', // redirect back to the signup page if there is an error
        failureFlash : true // allow flash messages
    }));

    // =====================================
    // PROFILE SECTION =====================
    // =====================================
    // we will want this protected so you have to be logged in to visit
    // we will use route middleware to verify this (the isLoggedIn function)
    app.get('/profile', isLoggedIn, function(req, res) {

        res.render('profile.ejs', {
            user : req.user, message: req.flash('profileMessage') // get the user out of session and pass to template
        });
    });

    //Geting the observation request form 
    app.get('/obs_request', isLoggedIn, isValid, function(req, res){

        res.render('obs_request.ejs', {user : req.user});
    });

    //Observation request form post
    app.post('/obs_request', isLoggedIn, isValid, function(req, res){

        var form = formidable.IncomingForm();

        // var fields

        // form.on('field', function(field, value){

        //     console.log(field);
        //     console.log(value);
        // });

        // form.on('end', function(){

        // });        

        form.parse(req, function (err, fields, files){

            console.log('parsing');

            fields.user_id = req.user.user_id;

            async.waterfall( [async.apply(db_query.insert_request, fields),
                db_query.insert_observer_request,
                db_query.insert_obs_seqs,
                db_query.select_filter,
                db_query.insert_filter_observation], 

            function(err, result){

                if(err) return err;

                //console.log('result = ' ,result);

                req.flash('profileMessage', 'Requête enregistrée');
                res.redirect('/profile');
            });
        });
    });



    app.get("/test",function(req, res){

        console.log("reception");
        console.log(req.cible);
    });



    // =====================================
    // LOGOUT ==============================
    // =====================================
    app.get('/logout', function(req, res) {
        req.logout();
        res.redirect('/');
    });



    // =====================================
    // ERROR HANDLING ======================
    // =====================================
    
    app.get('/404', function(req, res, next){
      // trigger a 404 since no other middleware
      // will match /404 after this one, and we're not
      // responding here
      next();
    });

    
    app.use(function(req, res, next){
        res.status(404);

        // respond with json
        if (req.accepts('json')) {
            
            res.send({ error: 'Not found' });
            return;
        }

        // default to plain-text. send()
        res.type('txt').send('Not found');
    });

};



    // =====================================
    // FUNCTIONS ===========================
    // =====================================

function process_obs_req(req, res){

    
}

// route middleware to make sure a user is logged in
function isLoggedIn(req, res, next) {

    // if user is authenticated in the session, carry on 
    if (req.isAuthenticated())
        return next();

    // if they aren't redirect them to the home page
    res.redirect('/');
}


function isValid(req, res, next){

    if( req.user.valid ) return next();

    res.redirect('/profile');
}