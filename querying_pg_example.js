/*pg.connect(connString, function(err, client, done){

  if(err){
    return console.log("Error while connecting to the pfe database : ", err);
  }

  client.query("ALTER TABLE Observer ADD COLUMN email CHAR(256) NOT NULL;" +
    "ALTER TABLE Observer ADD COLUMN user_project_desc TEXT NOT NULL;", function(err, result){

      done();

      if(err){

        return console.log("Error while running query : ", err);
      }
    });
});*/



/*// Query for creating tables on the DB
var sql_query = fs.readFileSync('../pfe_db.sql').toString();

var client = new pg.Client(connString);

client.connect(function(err, client){

  if(err){
    return console.log("Error while connecting to the pfe database : ", err);
  }
 
  client.query(sql_query, function(err, result){

    if(err){
      return console.log("Error on query : ", err);
    }

    console.log("Tables created succefully.");
  });
});*/