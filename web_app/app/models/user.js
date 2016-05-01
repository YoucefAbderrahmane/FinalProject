var bcrypt   = require('bcrypt');
var Sequelize = require('sequelize');

//Instanciating the sequelizer
var db_config = require('../../config/database.js');
var sequelize = new Sequelize(db_config.seq_connString, {define: { timestamps : false}});

var User = sequelize.define('observer', {

  user_id: {
  	type: Sequelize.INTEGER,
    primaryKey: true, // also allows for composite primary keys, even though the support for composite keys accross associations is spotty
    autoIncrement: true
  },
  email: Sequelize.STRING,
  password: Sequelize.STRING,
  valid: Sequelize.BOOLEAN
},{

	tableName: 'observer',

	classMethods: {

		generateHash : function(password, callBack){

			bcrypt.genSalt(8, function(err, salt) {

   				bcrypt.hash(password, salt, callBack);
			});
		}
	},
	instanceMethods: {
      	
      	validPassword : function(password, callBack){

      		bcrypt.compare(password, this.password, callBack);
		}
    }
});

module.exports = User;