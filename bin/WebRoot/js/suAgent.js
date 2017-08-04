//deployed js for suAgent
//2013-5-4

function status_callBack(r)
{
	alert(r);
}

function shutdownServer()
{
	var pl = new SOAPClientParameters();
	pl.add("cmd", "Stop");
	var url = document.location.href.replace("main.html", "ServerManager");	
	//url = "http://127.0.0.1:5001/ServerManager";
	SOAPClient.invoke(url, "run", pl, true, status_callBack);
}

// Test save profit data to sqlite
function saveProfitToDB()
{
	var pl = new SOAPClientParameters();
	pl.add("cmd", "Stop");
	var url = document.location.href.replace("main.html", "RecProfits");	
	//url = "http://127.0.0.1:5001/RecProfits";
	SOAPClient.invoke(url, "run", pl, true, status_callBack);
}