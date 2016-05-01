var pg = require('pg'),
async = require('async');

var Request            = require('../app/models/request');
var db_config = require('./database.js');

module.exports = {

	//DONE
	select_filter : function(param, callBack){

		var select_filter = "SELECT * FROM filter_type";

		param.fields.client.query(select_filter, function(err, filters) {

            if(err) {

                throw err;
            }

            async.waterfall(
            	[function(func){

            		param.fields.filters = filters.rows;

		            func(null, param.fields);
            	}], 

            	function(err, fields){

            		if(err) return err;

            		if( callBack != null ){

			            callBack(null, {fields : fields});
			        }
			        else{

			            return filters.rows;
			        }
            });
        });
	},


    //insert one observation sequence
    insert_observation_seq : function(param){

    	query = ins_obs_seq = "INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id";

    	param.fields.client.query(query,

    		[Request.isNull(param.fields.obs_exp_time), 
        	param.fields.request_id],

        	function(err, obs){

        		if(err) throw err;

        		return obs.rows[0].observation_id;
        	} 
        );
    },

    //insert one observation-filter
    insert_obs_filter : function(param){

    	ins_obs_filter = "INSERT INTO observation_filter(filter_type_id, request_id, observation_id, filter_user_exposure_time) values($1, $2, $3, $4) returning *";
	    
	    client.query(
	        ins_obs_filter,	//query

	        [param.fields.filter_type_id, //attributes (filter_id, req_id, obs_id, filter_exp)
	        param.fields.request_id, 
	        param.fields.observation_id, 
	        Request.isNull(param.fields.filter_exp)], 

	        function(err, result){	//callback

		           if(err){
		              throw err;
		           }

		        return result.rows[0];
	       });
    },

	//DONE
	insert_request : function(fields, callBack){

		var obj = this;

    	var request = Request.build({
                request_name: fields.request_name,
                request_target: fields.object,
                target_ra: Request.h_min_sec_to_deg(fields.AD_h, fields.AD_min, fields.AD_sec),
                target_dec: Request.deg_min_sec_to_deg(fields.Dec_deg, fields.Dec_min, fields.Dec_sec),
                observation_interval_start: Request.isNull(fields.obs_time_start),
                observation_interval_end: Request.isNull(fields.obs_time_start),
                observation_repetition: Request.isNull(fields.nb_shot),
                observation_period: Request.isNull(fields.period),
                minimum_height: Request.isNull(fields.min_moon_height),
                minimum_moon_distance: Request.isNull(fields.min_moon_dist)
        	})
            .save()
            .then(function(request){

            	pg.connect(db_config.connString, function(err, client, done){

            		if(err) throw err;

            		async.waterfall(
            			[function(func){

            				fields.client = client;
		            		fields.request_id = request.request_id;

		            		func(null, fields);
            		}], 

            		function(err, fields){

            			if(err) return err;

            			if( callBack != null ){

            				//console.log(fields);
            				callBack(null, {fields : fields});
            				done();
            			}
		            	else{

		            		done();
		            		return request;
		            	}
            		});
            	});

        	}).catch(function(err){

        		done();
        		throw err;
        	});
    },

    //DONE
	insert_observer_request : function(param, callBack){

        var query = "INSERT INTO observer_request(user_id, request_id) values($1, $2)";

        param.fields.client.query(query,
        	[param.fields.user_id, 
        	param.fields.request_id], 

        	function(err, result){

	            if(err){
	            		
	            	return err;
	            }

	            callBack(null, param);
        });
    },


    insert_obs_seqs : function(param, callBack){

    	if('sans_f' in param.fields && param.fields.sans_f == 'on'){

    		for (var i = 0; i < param.fields.nb_shot; i++) {
    		
	    		query = ins_obs_seq = "INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id";

		    	param.fields.client.query(query,

		    		[Request.isNull(param.fields.obs_exp_time), 
		        	param.fields.request_id],

		        	function(err, obs){

		        		if(err) throw err;

		        		return obs.rows[0].observation_id;
		        	} 
		        );
    		};
    	}

    	if( callBack != null) callBack(null, param);
    },


    insert_obs_filters : function(param){

    	var i = 0;

    	async.whilst(

    		function(){

    			return i < param.fields.nb_shot;
    		},

    		function(callfunc){

    			i++;
    			
    			//insert_observation_seq(param);
    			
    			callfunc(null, i);
    		},

    		function(err, n){

    			return i;
    		}

    	);
    },


    //DONE
    insert_observation : function(param, callBack){

    	ins_obs_seq = "INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id";
       	
        param.fields.client.query(ins_obs_seq, 
        	[Request.isNull(param.fields.obs_exp_time), 
        	param.fields.request_id], 

        	function(err, observation){

	        	if(err) throw err;

	        	async.waterfall(
	            	[function(func){

	            		param.fields.observation_id = observation.rows[0].observation_id;

			            func(null, param.fields);
	            	}], 

	            	function(err, fields){

	            		if(err) return err;

	            		if( callBack != null ){

	            			//console.log(fields);
	            			callBack(null, {fields : fields});
	            		}
			            else{

			            	return observation;
			            }
	            });
        });
    },

    insert_filter_observation : function(param, callBack){

    	for (var i = param.fields.filters.length - 1; i >= 0; i--) {
    		
    		var filter = param.fields.filters[i];

    		if( filter.filter_type_name in param.fields ){

    			if( param.fields[filter.filter_type_name] == 'on'){

    				for( var j = 0; j < param.fields.nb_shot; j++){

    					ins_obs_seq = "INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id";
                                
                    	param.fields.client.query(
                    		ins_obs_seq, //query

                    		[param.fields.obs_exp_time, //attributes
                    		param.fields.request_id], 
                    		
                    		function(err, obs){	//callback

	                    		if(err){ 
	                           		throw err;
	                    		}

	                    		ins_obs_filter = "INSERT INTO observation_filter(filter_type_id, request_id, observation_id, filter_user_exposure_time) values($1, $2, $3, $4)";
	                            param.fields.client.query(
	                            	ins_obs_filter,	//query

	                            	[filter.filter_type_id, //attributes
	                            	param.fields.request_id, 
	                            	obs.rows[0].observation_id, 
	                            	Request.isNull(param.fields['tmp_exp_' + filter.filter_type_name])], 

	                            	function(err, result){	//callback

		                                if(err){

		                                    throw err;
		                                }
	                            });
                    	});
    				}
    			}
    		}
    	}

    	if(callBack != null) callBack(null, param);
    }
};