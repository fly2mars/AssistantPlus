var currentFormVisibilityStatus = false;
function SaveNotes() {
    var category = document.getElementById("slSearchCategory").value;
    var todo = document.getElementById("txtToDo").value;

    if (category == "") {
	alert("Please select Category.");
	return;
    }

    var storage = JSON.parse(localStorage.getItem('ToDoList'));
    var arrayLength = storage.length;

    storage[arrayLength] = category;
    storage[arrayLength + 1] = todo;
    localStorage.setItem('ToDoList', JSON.stringify(storage));
    category = "";
    loadNotes();
    clearNote();
}

function clearNote() {
    var todo = document.getElementById("txtToDo");
    todo.value = '';
}

function loadNotes() {
    var storage = JSON.parse(localStorage.getItem('ToDoList'));

    if (!storage) {
	storage = [];
	localStorage.setItem('ToDoList', JSON.stringify(storage));
    }

    var displayArea = document.getElementById("displayArea");
    var currentFilter = document.getElementById("slSearchCategory").value;
    var innerDiv = "";
    for (var i = storage.length - 1; i >= 0; i = i - 2) {
	if (currentFilter == storage[i - 1] || currentFilter == "") {
	    var todoColor = 'AD7460';
	    switch (storage[i - 1]) {
		case 'Personal':
		    todoColor = '90CA77';
		    break;
		case 'Work':
		    todoColor = '81C6DD';
		    break;
		case 'Temp':
		    todoColor = 'F9D654';
		    break;
		case 'Public':
		    todoColor = 'AD7460';
		    break;
		default:
		    todoColor = 'AD7460';
		    break;
	    }
	    innerDiv += "<div class='displayToDo'  style='background:#" + todoColor + "'><input type='image' src='css/delete.png' width='15px' height='15px' onclick='removeMe(" + i + ")' /> Category : " + storage[i - 1] + " <hr /> " + storage[i] + "</div>";
	}
    }

    if (innerDiv != undefined) {
	displayArea.innerHTML = innerDiv;
    }
    else {
	displayArea.innerHTML = "";
    }
}

function removeMe(itemId) {
    var storage = JSON.parse(localStorage.getItem('ToDoList'));
    storage.splice(itemId - 1, 2);
    localStorage.setItem('ToDoList', JSON.stringify(storage));
    loadNotes();
}


onload = function () {
    loadNotes();
    ShowHideForm();
}