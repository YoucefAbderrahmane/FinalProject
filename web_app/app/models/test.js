// var request = require('request');

// request.get("http://cdsweb.u-strasbg.fr/cgi-bin/nph-sesame/-ox/~SNVA?vega", function(err, res, body){
        
//         if(!err){
            
//             console.log(body);
//         } else {
            
//             console.log(err);
//         }
// 	});


// var XHRrequest= new XMLHttpRequest();
	 	
// 	XHRrequest.onabort = function() {
	
// 	};
		
// 	XHRrequest.onloadend = function (evt) {
	
// 	};
	
// 	XHRrequest.onreadystatechange = function() {
		
// 		if (XHRrequest.readyState == 4 && XHRrequest.status == 200) {
// 	   	//parse XHRrequest.responseText for response and build object
// 	   	//this is where you update your UI with an HTML element
// 		}
// 	  	else if (XHRrequest.readyState == 4 && XHRrequest.status!=0) {
// 	   		console.log("Error: returned status code " + XHRrequest.status + " " + XHRrequest.statusText + "\r\n" + XHRrequest.responseText);
// 	   		//error handling for failed response
// 	  	}
// 	};
	
// 	XHRrequest.ontimeout = function() {
	  	
// 	  	alert("Error: event timed out");
// 	};
	
// 	XHRrequest.open("GET", "http://cdsweb.u-strasbg.fr/cgi-bin/nph-sesame/-ox/~SNVA", true);
// 	XHRrequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
// 	XHRrequest.send('vega'); //request data goes here



// var pg = require('pg');

// pg.connect('pg://pfe:pfe@localhost/pfe', function(err, client, done){

//     if(err){
//         done();
//         throw err;
//     }

//     client.query("INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id",
//                     [20, 10],

//                     function(err, result) {
//                         //call `done()` to release the client back to the pool

//                          //done();

//                         if(err) {
//                           throw err;
//                         }
//                         console.log('1 = ' + result.rows[0].observation_id);
//                         //output: 1

//                         client.query("INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id",
//                     [20, 10],

//                             function(err, res) {
//                                 //call `done()` to release the client back to the pool



//                                 //done();

//                                 if(err) {
//                                   throw err;
//                                 }
//                                 console.log(res.rows[0].observation_id);
//                                 //output: 1
//             				});

                       
//     });

//     //console.log(res[0]);
// });




h_min_sec_to_deg = function(hour, min, sec){

	var h = parseInt(hour*15);
	var m = parseFloat(min*(15/60));
	var s = parseFloat(sec*(15/3600));

	if(h < 0){

		return h - m - s;
	}
	else{

		return h + m + s;
	}
}

console.log(h_min_sec_to_deg(18, 36, 56.33635));

deg_min_sec_to_deg = function(deg, min, sec){

	var d = parseInt(deg);
	var min = parseFloat(min/60);
	var sec = parseFloat(sec/3600);

	if(d < 0){

		return d - min - sec;
	}
	else{

		return d + min + sec;
	}
}

// console.log(h_min_sec_to_deg('-1','45',''));
console.log(deg_min_sec_to_deg('38','47','01.2802'));

// { fields: 
//    { request_name: 'name',
//      object: 's',
//      AD_h: '0',
//      AD_min: '0',
//      AD_sec: '0',
//      Dec_deg: '0',
//      Dec_min: '0',
//      Dec_sec: '0',
//      obs_time_start: '',
//      obs_time_end: '',
//      obs_type: '',
//      period: '',
//      nb_shot: '1',
//      filterA: 'false',
//      tmp_exp_A: '',
//      tmp_exp_B: '',
//      tmp_exp_C: '',
//      tmp_exp_D: '',
//      tmp_exp_E: '',
//      min_moon_height: '',
//      min_moon_dist: '' },
//   files: {} }

            // var request = Request.build({
            //     request_name: fields.request_name,
            //     request_target: fields.object,
            //     target_ra: Request.h_min_sec_to_deg(fields.AD_h, fields.AD_min, fields.AD_sec),
            //     target_dec: Request.deg_min_sec_to_deg(fields.Dec_deg, fields.Dec_min, fields.Dec_sec),
            //     observation_interval_start: Request.isNull(fields.obs_time_start),
            //     observation_interval_end: Request.isNull(fields.obs_time_start),
            //     observation_repetition: Request.isNull(fields.nb_shot),
            //     observation_period: Request.isNull(fields.period),
            //     minimum_height: Request.isNull(fields.min_moon_height),
            //     minimum_moon_distance: Request.isNull(fields.min_moon_dist)
            // })
            // .save()
            // .then(function(request){

            //     pg.connect(db_config.connString, function(err, client, done){

            //         if(err){

            //             done();
            //             throw err;
            //         }

            //         var query = "INSERT INTO observer_request(user_id, request_id) values($1, $2)";

            //         client.query(query, [req.user.user_id, request.request_id]);


            //         if( ('Filter' + filter) in fields) {
                        
                        //     if( fields.filterA == 'on'){

                        //         for( var i = 0; i < request.observation_repetition; i++){

                        //             ins_obs_seq = "INSERT INTO observation_sequence(exposure_time, request_id) values($1, $2) returning observation_id";
                                                
                        //             client.query(ins_obs_seq, [fields.obs_exp_time, request.request_id], function(err, obs){

                        //                 if(err){ 
                        //                    throw err;
                        //                 }

                        //                 var select_filter = "SELECT filter_type_id FROM filter_type WHERE filter_type_name = $1";

                        //                 client.query(select_filter, [('Filter ' + filter)], function(err, result) {

                        //                     if(err) {

                        //                         throw err;
                        //                     }

                        //                     ins_obs_filter = "INSERT INTO observation_filter(filter_type_id, request_id, observation_id, filter_user_exposure_time) values($1, $2, $3, $4)";
                        //                     client.query(ins_obs_filter, [result.rows[0].filter_type_id, request.request_id, obs.rows[0].observation_id, Request.isNull(fields['tmp_exp_' + filter])], function(err, result){

                        //                         if(err){
                        //                             throw err;
                        //                         }
                        //                     });
                        //                 });
                        //             });
                        //         }
                        //     }
                        // }
            //     });

            //     req.flash('profileMessage', 'Requête enregistrée');
            //     res.redirect('/profile');
            // })
            // .catch(function(err){

            //     throw err;
            // });

            // console.log('checked boxes : ' + req.param('filters[]').length);

            // res.writeHead(200, {
            //     'content-type': 'text/plain'
            // });
            // res.write('received the data:\n\n');
            // res.end(util.inspect({
            //     fields: fields,
            //     files: files
            // }));