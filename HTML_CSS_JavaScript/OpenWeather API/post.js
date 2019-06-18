
function sendIt() {
	var userIn = document.getElementById("userEnteredText").value;
	var req = new XMLHttpRequest();
	req.open("POST", "http://httpbin.org/post", true);
	req.setRequestHeader('Content-Type', 'application/json');
	req.addEventListener("load", function(){
		if (req.status >= 200 && req.status < 400){
			var response = JSON.parse(req.responseText);
			document.getElementById("postResponse").textContent = response.data;
		}
		else
			console.log("ERROR IN NETWORK REQUEST");
	});
	req.send(JSON.stringify(userIn));
	event.preventDefault();
}

// Create button
var submitBut = document.createElement("button");
submitBut.style.width = "100px";
submitBut.style.height = "50px";
submitBut.style.marginTop = "20px";
submitBut.addEventListener("click", sendIt);
submitBut.appendChild(document.createTextNode("Submit Text"));
document.body.appendChild(submitBut);