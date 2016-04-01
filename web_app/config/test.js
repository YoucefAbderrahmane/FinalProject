var User            = require('../app/models/user');

User.findOne({where:{ 'email' :  'by_bekhti@esi.dz' }}).then(function(user) {

            // if no user is found, return the message
            if (!user)
                return console.log("no user"); // req.flash is the way to set flashdata using connect-flash

            var res = user.validPassword('kvih');
            console.log("result from user " + res)
            if( !res ){

                return console.log("mot de passe incorrect"); 
                // create the loginMessage and save it to session as flashdata
            }
            else{

                // all is well, return successful user
                return console.log("bsahtek");
            }
            

        })
        .catch(function(err){

            return console.log(err);
        });