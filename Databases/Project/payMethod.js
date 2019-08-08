module.exports = function(){
    var express = require('express');
    var router = express.Router();

	function getPayMethod(res, mysql, context, complete){
        mysql.pool.query("SELECT PayID, Type, Details FROM PaymentMethod", function(error, results, fields){
        if(error){
                res.write(JSON.stringify(error));
                res.end();
            }
            context.payMethods = results;
            complete();
        });
    }

    router.get('/', function(req, res){
        var callbackCount = 0;
        var context = {};
        //context.jsscripts = ["deleteperson.js"];
        var mysql = req.app.get('mysql');
        getPayMethod(res, mysql, context, complete);
        function complete(){
            callbackCount++;
            if(callbackCount >= 1){
                res.render('payment-methods', context);
            }
        }
    });

    return router;
}();