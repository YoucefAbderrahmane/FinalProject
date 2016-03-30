var express = require('express');
var cookieParser = require('cookie-parser');
var bodyParser   = require('body-parser');
var morgan = require('morgan');

var formidable = require('formidable');
var util = require('util');
var pg = require('pg');
var fs = require('fs');
var passport = require('passport');
var flash    = require('connect-flash');
var session = require('express-session');
var bodyParser   = require('body-parser');

require('../config/passport')(passport);

var router = express.Router();


router.use(morgan('dev'));
router.use(cookieParser()); // read cookies (needed for auth)
router.use(bodyParser.json()); // get information from html forms
router.use(bodyParser.urlencoded({ extended: true }));

router.use(session({secret: 'somesecret',resave: true, saveUninitialized: true}));
router.use(passport.initialize());
router.use(passport.session());
router.use(flash());


//Database connection
//var connString = "pg://pfe:pfe@localhost/pfe";
//pg.connect(connString);




/* GET signup page. */
router.get('/', function(req, res, next) {

	res.render('signup', { message: req.flash('signup message') });
});


// Post the form
router.post('/', function(req, res, next){

	console.log('processing...');
	passport.authenticate('local-signup', function(err, user, info){

		if(err){

			console.log('passport error :'+ err.message);
			return next(err);
		}

		if(!user){

			console.log('the user already exists!');
			return res.redirect('/signup');
		}
		else{

			return res.redirect('/');
		}

		
	})(req, res, next);
});

/*router.post('/signup', function(req, res, next){

	console.log('processing...' + req.body);
	passport.authenticate('local-signup', {
        successRedirect : '/', // redirect to home
        failureRedirect : '/signup', // redirect back to the signup page if there is an error
        failureFlash : true
    } )(req, res, next);
},
  function(err, req, res, next) {
    // failure in login test route
    return res.send({'status':'err','message':err.message});
  });*/

/*function(err, user, info){

		if(err){

			console.log('passport error :'+ err);
			return next(err);
		}

		if(user){

			console.log('the user already exists!');
			return res.redirect('/signup');
		}

		return res.redirect('/profile');

	}
*/
//export the router
module.exports = router;



//Functions

function process_signup(req, res){



	console.log("processing signup!");



	/*var form = formidable.IncomingForm();

	form.parse(req, function (err, fields, files){

		res.writeHead(200, {
            'content-type': 'text/plain'
        });
        res.write('received the data:\n\n');
        res.end(util.inspect({
            fields: fields,
            files: files
        }));
	});*/
}

