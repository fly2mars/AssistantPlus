
//var url = "http://127.0.0.1:5001/null";


// Test set global variable
function SetGVar()
{
	var pl = new SOAPClientParameters();
	pl.add("cmd", document.frmDemo.txtName.value);
	var url = document.location.href.replace("manage.html", "setgvar");	
	//url = "http://127.0.0.1:5001/setgvar";
	SOAPClient.invoke(url, "run", pl, true, SetGVar_callBack);
}
function SetGVar_callBack(r)
{
	alert(r);
}

// Test get global variable value
function GetGVar()
{
		var pl = new SOAPClientParameters();
		pl.add("cmd", document.frmDemo.vName.value);
		//url = "http://127.0.0.1:5001/getgvar";		
		var url = document.location.href.replace("manage.html", "getgvar");		
		SOAPClient.invoke(url, "run", pl, true, GetGVar_callBack);
}
function GetGVar_callBack(r)
{
		alert(r);
}


// utils
function toggle(id)
{
	var d = document.getElementById(id);
	if(d != null)
		d.className = (d.className == "h") ? "s" : "h";
}

// Test set global variable
function shutdownServer()
{
	var pl = new SOAPClientParameters();
	pl.add("cmd", "Stop");
	var url = document.location.href.replace("manage.html", "ServerManager");	
	//url = "http://127.0.0.1:5001/ServerManager";
	SOAPClient.invoke(url, "run", pl, true, SetGVar_callBack);
}

// Test save profit data to sqlite
function saveProfitToDB()
{
	var pl = new SOAPClientParameters();
	pl.add("cmd", "Stop");
	var url = document.location.href.replace("manage.html", "RecProfits");	
	//url = "http://127.0.0.1:5001/RecProfits";
	SOAPClient.invoke(url, "run", pl, true, SetGVar_callBack);
}