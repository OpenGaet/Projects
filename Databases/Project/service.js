module.exports = function(){
    var express = require('express');
    var router = express.Router();

	function getServices(res, mysql, context, complete){
        mysql.pool.query("SELECT SrvID, Type, Price FROM Service", function(error, results, fields){
        if(error){
                res.write(JSON.stringify(error));
                res.end();
            }
            context.services = results;
            complete();
        });
    }

    router.get('/', function(req, res){
        var callbackCount = 0;
        var context = {};
        //context.jsscripts = ["deleteperson.js"];
        var mysql = req.app.get('mysql');
        getServices(res, mysql, context, complete);
        function complete(){
            callbackCount++;
            if(callbackCount >= 1){
                res.render('services', context);
            }
        }
    });

    return router;
}();