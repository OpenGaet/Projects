module.exports = function(){
    var express = require('express');
    var router = express.Router();

	function getReservations(res, mysql, context, complete){
        mysql.pool.query("SELECT ResID, CheckIn, CheckOut, TotalOwed, GuestID, PayID FROM Reservation", function(error, results, fields){
        	if(error){
                res.write(JSON.stringify(error));
                res.end();
            }
            context.reservations = results;
            complete();
        });
    }

    router.get('/', function(req, res){
        var callbackCount = 0;
        var context = {};
        //context.jsscripts = ["deleteperson.js"];
        var mysql = req.app.get('mysql');
        getReservations(res, mysql, context, complete);
        function complete(){
            callbackCount++;
            if(callbackCount >= 1){
                res.render('reservations', context);
            }
        }
    });

    return router;
}();