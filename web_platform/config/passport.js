var LocalStrategy   = require('passport-local').Strategy;


//getting the user model
var User = require('../models/user');


//export the passport function to signup
module.exports = function(passport){

	passport.serializeUser(function(user, done){

		console.log("Serializing...");
		done(null, user.id);
	});

	passport.deserializeUser(function(err, user){

		User.findById(id).then(function(err, user) {
            
            console.log("Deserializing...");
            done(err, user);
        });
	});

	passport.use('local-signup', new LocalStrategy({

		usernameField : 'email',
		passwordField : 'password',
		passReqToCallback : true
	},
	function(req, email, password, done){

			console.log("Looking for an existing email...");

		

		// User.findOne wont fire unless data is sent back
		process.nextTick(function(){

			// find a user whose email is the same as the forms email
			User.findOne({where: {email: email}}).then(function(user){

				/*if(err){
					console.log(err.body);
					return done(err);
				}*/

				if(user){

					console.log('exists');
					return done(null, false, req.flash('signup message', 'Adresse mail utilisée.'));
				}
				else {

					console.log('ooohooo');

					var newUser = User.build({

						email: email,
						password: User.generateHash(password)
					});
					
					newUser.save().then(function(err) {
  						
  						if(err) throw err;
  						return done(null, newUser);
					});
				}
			});
		});
	}));
}