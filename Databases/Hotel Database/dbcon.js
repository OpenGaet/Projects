var mysql = require('mysql');
var pool = mysql.createPool({
  connectionLimit : 10,
  host            : 'classmysql.engr.oregonstate.edu',
  user            : 'cs340_tuckerra',
  password        : '7595',
  database        : 'cs340_tuckerra'
});
module.exports.pool = pool;
