var express = require('express');
var handlebars = require('express-handlebars').create({defaultLayout:'main'});
var mysql = require('./dbcon.js');
var bodyParser = require('body-parser');

var app = express();
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());
app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 4504);
app.use(express.static('public'));

app.get('/', function(req, res, next){

  res.render('home'); 
});

app.post('/', function(req, res){
  
  res.render('home');
});

app.get('/table',function(req, res, next){
    var content = {};
    mysql.pool.query('SELECT * FROM workouts', function(err, rows, fields){
        if(err){
            next(err);
            return;
        }
        content.results = JSON.stringify(rows);
        res.send(content);
    });
});

app.get('/insert', function(req, res, next){
  var content = {};
  mysql.pool.query('INSERT INTO workouts (`name`, `reps`, `weight`, `date`, `lbs`) VALUES (?, ?, ?, ?, ?)', [req.query.name, req.query.reps, req.query.weight, req.query.date, req.query.lbs], function(err, result){
    if(err){
      next(err);
      return;
    }
    content.results = "ID: " + result.insertId;
    res.send(content);
  });
  /*
   mysql.pool.query('SELECT * FROM workouts', function(err, rows, fields){
    if(err){
      next(err);
      return;
    }
    res.send(rows);
  });
  */
});

app.get('/update', function(req, res, next){
  var content = {};
  
      mysql.pool.query('UPDATE workouts SET name=?, reps=?, weight=?, date=?, lbs=? WHERE id=?', [req.query.name/*|| item.name*/, req.query.reps/* || item.reps*/, req.query.weight/* || item.weight*/, req.query.date/* || item.date*/, req.query.lbs/* || item.lbs*/, req.query.id], function(err, result){
                          if (err){
                            next(err);
                            return;
                          }
                          content.results = "Edited " + result.changedRows + " rows.";
                          res.send(content);
                        });
});

app.get('/delete', function(req, res, next){
  var content = {};
  mysql.pool.query('DELETE FROM workouts WHERE id=?', [req.query.id], function(err, result){
    if (err) {
      next(err);
      return;
    }
    content.results = "Removed: " + result.removeId;
    res.send(content);
  });
});

app.get('/reset-table', function(req,res,next){
  var content = {};
  mysql.pool.query("DROP TABLE IF EXISTS workouts", function(err){
    var createString = "CREATE TABLE workouts("+
    "id INT PRIMARY KEY AUTO_INCREMENT,"+
    "name VARCHAR(255) NOT NULL,"+
    "reps INT,"+
    "weight INT,"+
    "date DATE,"+
    "lbs BOOLEAN)";
    mysql.pool.query(createString, function(err){
      content.results = "Table reset";
      res.render('home', content);
    })
  });
});

app.use(function(req, res){
  res.status(404);
  res.render('error404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.status(500);
  res.render('error500');
});

app.listen(app.get('port'), function(){
  console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});