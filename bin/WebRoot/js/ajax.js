function Ajax(url, args) {
    this.url = url || "";
    this.params = args.parameters || "";
    this.mime = args.mime || "text/html";
    this.onComplete = args.onComplete || this.defaultOnCompleteFunc;
    this.onLoading= args.onLoading || this.defaultOnLoadingFunc;
    this.onError = args.onError || this.defaultOnErrorFunc;
    this.method = args.method || "post";
    //REMIND: yinwm -- 2007/03/30  Cost to much code, refine it later.
    if (typeof(args.sync) == "undefined" || args.sync == null) { 
	this.sync = true;
    } else {
	this.sync = args.sync ? true : false; //NOTE: yinwm -- 2007/03/30 Convert all other ones like string, number to boolean value.
    }
    this.loadData();
}

Ajax.prototype = {
    READY_STATE_COMPLETE : 4,
    getRequest : function () {
	var funcs = [
	    function() {return new ActiveXObject('Msxml2.XMLHTTP')},
	    function() {return new ActiveXObject('Microsoft.XMLHTTP')},
	    function() {return new XMLHttpRequest()},
	];

	var req = null;
	for (var i = 0; i < funcs.length; i++) {
	    var f = funcs[i];
	    try {
		req = f();
		break;
	    } catch (e) {}
	}

	return req || false;
    },

    //NOTE: yinwm -- convert paramater map to string 
    parseParams : function () {
	if (typeof (this.params) == "string") {
	    return this.params;
	} else {
	    var s = "";
	    for (var k in this.params) {
		s += k + "=" + this.params[k] + "&";
	    }
	    return s;
	}
    },

    loadData : function () {
	this.req = this.getRequest();
	
	if (this.req) {
	    this.onLoading();
	    try {
		var loader = this;
		this.req.onreadystatechange = function () {
		    if (loader.req.readyState == loader.READY_STATE_COMPLETE) {
			loader.onComplete.call(loader, loader.req);
		    }
		}
		this.req.open(this.method, this.url, this.sync);

		if (this.method == "post") {
		    this.req.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		}

		if (this.req.overrideMimeType) {
		    this.req.overrideMimeType(this.mime);
		}
		this.req.send(this.method == "post" ? this.parseParams(this.params) : null);
	    } catch (e) {
		// throw e
		this.onError.call(this, e);
	    }
	}
    },

    defaultOnCompleteFunc : function () {
	alert(this.req.responseText);
    },

    defaultOnLoadingFunc : function () {
    },

    defaultOnErrorFunc : function (error) {
	//alert(error);
    }
}


