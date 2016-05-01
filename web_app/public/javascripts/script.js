// document.getElementById('per').setAttribute('disabled', true);

document.getElementById("obj").addEventListener("change", get_coordinates);

function get_coordinates(){

	var x = document.getElementById("obj").value;
    //document.getElementById("demo").innerHTML = "You wrote: " + x;

	var XHRrequest = new XMLHttpRequest();

	var doc = document;
	
	XHRrequest.onreadystatechange = function() {
		
		if (XHRrequest.readyState == 4 && XHRrequest.status == 200) {
	   	//parse XHRrequest.responseText for response and build object
	   	//this is where you update your UI with an HTML element

			var xmlDoc = XHRrequest.responseXML;
		    //var info = xmlDoc.getElementsByTagName("INFO");
		    //info[0].childNodes[0].nodeValue
		    var ra = xmlDoc.getElementsByTagName("jradeg");
		    var dec = xmlDoc.getElementsByTagName("jdedeg");
		    if( ra[0] != 'undefined' ){

		    	var deg = ra[0].childNodes[0].nodeValue;

		    	var hour = parseInt(deg / 15);
		    	var min = parseInt( (deg - hour*15) * 60 / 15 );
		    	var sec = (deg - (hour*15) - (min*15/60)) * 3600 / 15;

		    	doc.getElementById("ad_h").value = hour;
		    	doc.getElementById("ad_min").value = min;
		    	doc.getElementById("ad_sec").value = sec;

		    	deg = dec[0].childNodes[0].nodeValue;

		    	var degrees = parseInt(deg);
		    	var minutes = parseInt( (deg - degrees)*60 );
		    	var seconds = (deg - degrees - (minutes/60))*3600;

				doc.getElementById("dec_deg").value = degrees;
		    	doc.getElementById("dec_min").value = minutes;
		    	doc.getElementById("dec_sec").value = seconds;		    	

			}
		}
	  	else if (XHRrequest.readyState == 4 && XHRrequest.status!=0) {
	   		console.log("Error: returned status code " + XHRrequest.status + " " + XHRrequest.statusText + "\r\n" + XHRrequest.responseText);
	   		//error handling for failed response
	  	}
	};
	
	XHRrequest.ontimeout = function() {
	  	
	  	alert("Error: event timed out");
	};
	
	XHRrequest.open("POST", "http://cdsweb.u-strasbg.fr/cgi-bin/nph-sesame/-ox/~SNVA", true);
	XHRrequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	XHRrequest.send(x);	
}


// document.getElementById("obs_type_id").addEventListener("change", get_obs_type);

// function get_obs_type(){

// 	var value = document.getElementById("obs_type_id").value;

// 	var doc = document;

// 	var per = document.getElementsById('per');
// 	//per.removeAttribute('disabled');
// 	per.setAttribute('disabled', false);

// 	// switch(value){

// 	// 	case "Périodique":
			
// 	// 		alert(document.getElementsById("per").getAttribute("disabled"));
// 	// 		document.getElementsById("reps").setAttribute("disabled", false);
// 	// 		break;
// 	// 	case "Répétée":
// 	// 		{
// 	// 			alert(value);
// 	// 			document.getElementsById("reps").disabled = false;
// 	// 		}
// 	// 		break;
// 	// 	default:
// 	// 		{
// 	// 			document.getElementsById("period").setAttribute("disabled", false);
// 	// 			document.getElementsById("nb_shot").setAttribute("disabled", false);
// 	// 		}
// 	// }

// 	//alert(value);
// }

// // document.getElementsById("obs_type_id").onchange = function(){

// // 	alert("observation type changed");
// // }