var bcrypt   = require('bcrypt-nodejs');
var Sequelize = require('sequelize');

//Instanciating the sequelizer
var connString = "postgres://pfe:pfe@localhost/pfe";
var sequelize = new Sequelize(connString);

var User = sequelize.define('observers', {

  //username: Sequelize.STRING,
  email: Sequelize.STRING,
  password: Sequelize.STRING
},{
	classMethods: {

		generateHash : function(password){

			console.log("Generating hash...");
			return bcrypt.hashSync(password, bcrypt.genSaltSync(8), null)
		},

		validPassword : function(password){

			console.log("Comparing password hashes");
			return bcrypt.compareSync(password, this.local.password)
		}
	}
});

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

module.exports = User;