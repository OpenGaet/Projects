var mysql = require('mysql');
var pool = mysql.createPool({
  connectionLimit : 10,
  host            : 'classmysql.engr.oregonstate.edu',
  user            : 'cs290_ingrassg',
  password        : 'BSgpKQMWH6SERF4P',
  database        : 'cs290_ingrassg'
});
module.exports.pool = pool;