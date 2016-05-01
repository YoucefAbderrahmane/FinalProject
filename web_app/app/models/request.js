var Sequelize = require('sequelize');

//Instanciating the sequelizer
var db_config = require('../../config/database.js');
var sequelize = new Sequelize(db_config.seq_connString, {define: { timestamps : false}});

var Request = sequelize.define('request', {

  request_id: {
  	type: Sequelize.INTEGER,
    primaryKey: true, // also allows for composite primary keys, even though the support for composite keys accross associations is spotty
    autoIncrement: true
  },
  request_name: Sequelize.STRING,
  request_target: Sequelize.STRING,
  target_ra: Sequelize.REAL,
  target_dec: Sequelize.REAL,
  request_state: Sequelize.ENUM
  	('pending',
  	'running',
    'done',
    'outdated'),
  duration: Sequelize.REAL,
  observation_interval_start: Sequelize.DATE,
  observation_interval_end: Sequelize.DATE,
  observation_repetition: Sequelize.INTEGER,
  observation_period: Sequelize.BIGINT,
  minimum_height: Sequelize.DOUBLE,
  minimum_moon_distance: Sequelize.DOUBLE

},{

	tableName: 'request',

	classMethods: {

		h_min_sec_to_deg : function(hour, min, sec){

			var h = parseInt(hour*15);
			var m = parseFloat(min*(15/60));
			var s = parseFloat(sec*(15/3600));

			if(h < 0){

				return h - m - s;
			}
			else{

				return h + m + s;
			}
		},

		deg_min_sec_to_deg : function(deg, min, sec){

			var d = parseInt(deg);
			var min = parseFloat(min/60);
			var sec = parseFloat(sec/3600);

			if(d < 0){

				return d - min - sec;
			}
			else{

				return d + min + sec;
			}
		},

		isNull : function(val){

			if( val == '' ) return null;
			else return val;
		}
	},
	instanceMethods: {
      	
    	
    }
});

module.exports = Request;