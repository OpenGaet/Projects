/////////////////////////////////////////////////////////////////////
function getData(event) {
  if (document.getElementsByName("name").value == "") {
            console.log("Error: Exercise is necessary");
            return;
        }

  var form = document.getElementById('newExercise');
  var req = new XMLHttpRequest();

  var input = "name=" + form.elements.name.value +
              "&reps=" + form.elements.reps.value +
              "&weight=" + form.elements.weight.value +
              "&date=" + form.elements.date.value;
  if (form.elements.unit.checked){
    input += "&lbs=1";
  }
  else {
    input += "&lbs=0";
  }

  if (form.elements.name.value === "") {
    console.log("ERROR: A name is necessary");
    return;
  }

  req.open("GET", "/insert?" + input, true);
  //req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  req.addEventListener('load', function(){
    if (req.status >= 200 && req.status < 400){
      var response = JSON.parse(req.responseText);
      // Create Table
      createTable()
    }
    else
      console.log("ERROR IN NETWORK REQUEST");
  });

  req.send(null);
  event.preventDefault();
}

////////////////////////////////////////////////////////////////
function deleteRow(id) {
  var req = new XMLHttpRequest();
  var input = "id=" + id;

  req.open("GET", "/delete?" + input, true);
  req.addEventListener('load', function(){
    if (req.status >= 200 && req.status < 400){
      var response = JSON.parse(req.responseText);
      // Create Table
      createTable()
    }
    else
      console.log("ERROR IN NETWORK REQUEST");
  });

  req.send(null);
  event.preventDefault();
}

///////////////////////////////////////////////////////////////
function updateRow(element) {

  document.getElementById("editExer").value = element["name"];
  document.getElementById("editReps").value = element["reps"];
  document.getElementById("editWeight").value = element["weight"];
  document.getElementById("editDate").value = element["date"].substring(0, 10);
  document.getElementById("unit").value = element["lbs"];

  document.getElementById('submitEdit').addEventListener('click', function(){
    var form = document.getElementById('newExercise');

    if (form.elements.name.value === "") {
    console.log("ERROR: A name is necessary");
    return;
    }

    var req = new XMLHttpRequest();

    var input = "name=" + form.elements.name.value +
              "&reps=" + form.elements.reps.value +
              "&weight=" + form.elements.weight.value +
              "&date=" + form.elements.date.value;
    if (form.elements.unit.checked){
      input += "&lbs=1";
    }
    else {
      input += "&lbs=0";
    }

  

    input = input + "&id=" + element["id"]; //.value

    req.open("GET", "/update?" + input, true);
    req.addEventListener('load', function(){
      if (req.status >= 200 && req.status < 400){
        var response = JSON.parse(req.responseText);
        // Create Table
        createTable()
      }
      else
        console.log("ERROR IN NETWORK REQUEST");
  });

  req.send(null);
  event.preventDefault();
  });
}

///////////////////////////////////////////////////////////////
function fillTable(input) {
  var tbody = document.createElement("tbody");
  tbody.id = "exercises";

  var types = ["name", "reps", "weight", "date", "lbs"];
  if(input != null){
    input.forEach(function(field){
      var row = document.createElement("tr");

      types.forEach(function(type){
        var item = document.createElement("td");
        if (field[type] != null){
          if (type === "date"){
            item.textContent = field[type].substring(0, 10);
          }
          else if (type === "lbs"){
            if (field[type]){
              item.textContent = "lbs";
            }
            else {
              item.textContent = "kg";
            }
          }
          else {
            item.textContent = field[type];
            //item.textContent = rowData[itemData];
          }
        }
        
        console.log(field[type]);
        row.appendChild(item);
      });

      var dummy = document.createElement("td");
      var form = document.createElement("form")

      var deleteButt = document.createElement("input")
      deleteButt.value = "Delete";
      deleteButt.type = "button";
      deleteButt.addEventListener("click", function(){deleteRow(field["id"]);});
      form.appendChild(deleteButt);

      var updateButt = document.createElement("input")
      updateButt.value = "Edit";
      updateButt.type = "button";
      updateButt.addEventListener("click", function(){updateRow(field);});
      form.appendChild(updateButt);
  
      var hiddenId = document.createElement("input")
      hiddenId.type = "hidden";
      hiddenId.id = field["id"];
      form.appendChild(hiddenId);
  
      dummy.appendChild(form);
      row.appendChild(dummy);

      tbody.appendChild(row);
    });

  }

  return tbody;
}

/////////////////////////////////////////////////////////////////////
function createTable() {
    var req = new XMLHttpRequest();

    req.open("GET", "/table", true);

    req.addEventListener("load", function(){
      var response = JSON.parse(req.responseText);
      var results = JSON.parse(response.results);
      console.log(results[0]);
      var newExercises = fillTable(results);

      var parentTable = document.getElementById("workTable");
      parentTable.removeChild(document.getElementById("exercises"));
      newExercises.id = "exercises";
      parentTable.appendChild(newExercises);
    });

    req.send(null);
    event.preventDefault();
}


var exerciseTable = fillTable(null);
var parentTable = document.getElementById("workTable");
parentTable.appendChild(exerciseTable);

document.addEventListener("DOMContentLoaded", function(){
  createTable();
  document.getElementById('submitWork').addEventListener('click', getData);
});
