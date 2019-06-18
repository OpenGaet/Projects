var slideNum = 0;

startSlides();

function startSlides(){

	var slides = document.getElementsByClassName("slides");

	var i;
	for (i = 0; i < slides.length; i++) {
    slides[i].style.display = "none"; 
    }

	slideNum++;

	if (slideNum > slides.length) {
		slideNum = 1;
	}

	slides[slideNum-1].style.display = "block";

	// Continue slides every 3 seconds
	setTimeout(startSlides, 3000);
}


function calc() {
	var age = document.getElementById("age").value;
	var total = document.getElementById("total").value;
	var perYear = 0;
	var perYearBank = 0;
	var years = 65 - age;
	var rateStock = 0.05;
	var rateBank = 0.03;
	var totalBank = Math.pow(1.03, years - 1) * total;

	perYear = total / (Math.pow(1 + rateStock, years - 1) / rateStock);
	perYear = Math.ceil(perYear * 100) / 100;

	perYearBank = totalBank / years;
	perYearBank = Math.ceil(perYearBank * 100) / 100;

	document.getElementById("perYearStock").innerHTML = "$" + perYear;
	document.getElementById("perYearBank").innerHTML = "$" + perYearBank;
}

// Create buttons
var submit = document.getElementById("submitInfo");
submit.style.width = "100px";
submit.style.height = "30px";
submit.style.marginRight = "20px";
submit.addEventListener("click", calc);
