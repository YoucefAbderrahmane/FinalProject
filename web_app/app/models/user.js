var bcrypt   = require('bcrypt');
var Sequelize = require('sequelize');

//Instanciating the sequelizer
var connString = "postgres://pfe:pfe@localhost/pfe";
var sequelize = new Sequelize(connString, {define: { timestamps : false}});

var User = sequelize.define('observer', {

  user_id: {
  	type: Sequelize.INTEGER,
    primaryKey: true, // also allows for composite primary keys, even though the support for composite keys accross associations is spotty
    autoIncrement: true
  },
  email: Sequelize.STRING,
  password: Sequelize.STRING
},{

	tableName: 'observer',

	classMethods: {

		generateHash : function(password, callBack){

			// console.log("Generating hash...");
			// return bcrypt.hashSync(password, bcrypt.genSaltSync(8), null);

			bcrypt.genSalt(8, function(err, salt) {

   				bcrypt.hash(password, salt, callBack);
			});
		}
	},
	instanceMethods: {
      	
      	validPassword : function(password){

			bcrypt.compare(password, this.password, function(err, res){

				if(err) throw err;
				return res;
			});
		}
    }
});

// User.findOne({where:{ 'id' :  7 }}).then(function( user) {
//             // if there are any errors, return the error before anything else
           

//             // if no user is found, return the message
//             if (!user)
//                 return console.log('no user');

//             // all is well, return successful user
//             return console.log(user.email + ' ' + user.password);
//         });

/*User.findOne({ where: {email: 'by_bekhti@esi.dz'} }).then(function(user) {
  // project will be the first entry of the Projects table with the title 'aProject' || null

  console.log(user.email + ' ' + user.password + ' ' + user.id);
});

var newUser = User.build({

						email: 'youcef@bekhti.dz',
						password: User.generateHash("youyou")
					});

					newUser.save().then(function(err) {
  						
  						if(err) throw err;
  						return console.log('done!');
					});*/

//User.sync();


			// bcrypt.compare('esi', '$2a$08$DclFPia6.ENfzBrKr.o2WebwgxFRvHZNf6ckNR3BV1jeC0IkcIeVO', function(err, res) {
                
   //              if(err) console.log(err.message);
   //              return console.log(res); 
   //          });

module.exports = User;