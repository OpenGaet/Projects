var userIn;

var userAPI = "...";

function getZipcode() {
	userIn = document.getElementById("location").value;
	var req = new XMLHttpRequest();
	req.open("GET", "http://api.openweathermap.org/data/2.5/weather?zip=" 
		+ userIn + "," + "us&appid=" + userAPI, true);
	req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	req.addEventListener("load", function(){
		if (req.status >= 200 && req.status < 400){
			var response = JSON.parse(req.responseText);
			document.getElementById("getResponse").textContent = response.weather[0].description;
		}
		else
			console.log("ERROR IN NETWORK REQUEST");
	});
	req.send(null);
	event.preventDefault();

}

function getCity() {
	userIn = document.getElementById("location").value;
	var req = new XMLHttpRequest();
	req.open("GET", "http://api.openweathermap.org/data/2.5/weather?q=" 
		+ userIn + "," + "us&appid=" + userAPI, true);
	req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	req.addEventListener("load", function(){
		if (req.status >= 200 && req.status < 400){
			var response = JSON.parse(req.responseText);
			document.getElementById("getResponse").textContent = response.weather[0].description;
		}
		else
			console.log("ERROR IN NETWORK REQUEST");
	});
	req.send(null);
	event.preventDefault();
}


// Create buttons
var zip = document.createElement("button");
zip.style.width = "100px";
zip.style.height = "50px";
zip.style.marginRight = "20px";
zip.addEventListener("click", getZipcode);
zip.appendChild(document.createTextNode("Submit Zip"));
document.body.appendChild(zip);

var city = document.createElement("button");
city.style.width = "100px";
city.style.height = "50px";
city.style.margin = "20px";
city.addEventListener("click", getCity);
city.appendChild(document.createTextNode("Submit City"));
document.body.appendChild(city);
