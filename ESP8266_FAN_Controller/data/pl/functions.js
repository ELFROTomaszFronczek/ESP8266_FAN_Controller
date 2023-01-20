var inReset=false;
var notAllowedChars = "\"\\\r\n";
var textChars = "01234567890. ~`!@#$^&*()_-=+{}|:<>?qwertyuiopasdfghjklzxcvbnm,.;'[]QWERTYUIOPASDFGHJKLZXCVBNM%";
var spacje = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
var errors = "";
var req2;
var req;

function myFunction() {
    document.getElementById("myDropdown").classList.toggle("show");
}

window.onclick = function (event) {
    if (!event.target.matches('.dropbtn')) {
        var dropdowns = document.getElementsByClassName("dropdown-content");
        var i;
        for (i = 0; i < dropdowns.length; i++) {
            var openDropdown = dropdowns[i];
            if (openDropdown.classList.contains('show')) {
                openDropdown.classList.remove('show');
            }
        }
    }
}



function textIsEmpty(text) { return (text == null || text == "" || text.length < 1); }

function isNum(text) { 
try{
if (textIsEmpty(text.toString())) return false;
return !isNaN(text);
} catch (e) {}
return false;
}

function ValidateIPaddress(ipaddress) {
    if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(ipaddress)) {
        return (true)
    }

    return (false)
}

function haveAllowedChars(text, list) {
    if (textIsEmpty(text)) return true;
    for (var i = 0; i < text.length; i++) {
        if (list.indexOf(text.charAt(i)) < 0) return false;
    }
    return true;
}


function haveNotAllowedChars(text, list) {
    if (textIsEmpty(text)) return false;
    for (var i = 0; i < text.length; i++) {
        if (list.indexOf(text.charAt(i)) > -1) return true;
    }
    return false;
}

function isChecked(ID)
{
	var obj = document.getElementById(ID);

    if (obj != null) {
		return obj.checked;
	}
	return false;
}

function testInt(ID,  errorText, emptyError = "", min = 0, max=100, rangeErr = "", addclass="") {

    var obj = document.getElementById(ID);

    if (obj != null) {
        obj.className = addclass;
		if (addclass!="") addclass=" "+addclass;
        var txt = obj.value;
        var rv = txt;

        
            if (textIsEmpty(txt)) {
                obj.className = "errorShadow"+addclass;
                errors = errors + spacje + emptyError + "<br/>";
                //   rv = "";
            }
            else
			{
				if (isNum(txt)){
					var i=parseInt(txt);
					if (i<min || i>max) {
                        obj.className = "errorShadow"+addclass;
                        errors = errors + spacje + rangeErr + "<br/>";
                    } else 
					rv=i;
                }
				else
				{
                    obj.className = "errorShadow"+addclass;
                    errors = errors + spacje + errorText + "<br/>";
				}
					
        }
        

        return rv;
    }
    return "";
}



function testText(ID, list, errorText, emptyError = "", canEmpty = true, min = 0, minErr = "", addclass="") {

    var obj = document.getElementById(ID);

    if (obj != null) {
        obj.className = addclass;
		if (addclass!="") addclass=" "+addclass;
        var txt = obj.value;
        var rv = txt;

        if (!canEmpty) {
            if (textIsEmpty(txt)) {
                obj.className = "errorShadow"+addclass;
                errors = errors + spacje + emptyError + "<br/>";
                //   rv = "";
            }
            else
                if (min > 0) {
                    if (txt.length < min) {
                        obj.className = "errorShadow"+addclass;
                        errors = errors + spacje + minErr + "<br/>";
                    }
                }
        }
        if (!haveAllowedChars(txt, list)) {
            obj.className = "errorShadow"+addclass;
            errors = errors + spacje + errorText + "<br/>";
            //   rv = "";
        }

        return rv;
    }
    return "";
}


function testText2(ID, list, errorText, emptyError = "", canEmpty = true, min = 0, minErr = "", addclass="") {

    var obj = document.getElementById(ID);

    if (obj != null) {
        obj.className = addclass;
		if (addclass!="") addclass=" "+addclass;
        var txt = obj.value;
        var rv = txt;

        if (!canEmpty) {
            if (textIsEmpty(txt)) {
                obj.className = "errorShadow"+addclass;
                errors = errors + spacje + emptyError + "<br/>";
                //   rv = "";
            }
            else
                if (min > 0) {
                    if (txt.length < min) {
                        obj.className = "errorShadow"+addclass;
                        errors = errors + spacje + minErr + "<br/>";
                    }
                }
        }
        if (haveNotAllowedChars(txt, list)) {
            obj.className = "errorShadow"+addclass;
            errors = errors + spacje + errorText + "<br/>";
            //   rv = "";
        }

        return rv;
    }
    return "";
}

function compareText(ID1, ID2, errorText, okText) {

    var obj1 = document.getElementById(ID1);
    var obj2 = document.getElementById(ID2);

    if (obj1 == null || obj2 == null) return false;
    if (obj1.value != obj2.value) {
        obj2.className = "errorShadow";
        errors = errors + spacje + errorText + "<br/>";
        return ""
    }
    return okText;

}


function testIP(ID, errorText, emptyError = "", canEmpty = true) {

    var obj = document.getElementById(ID);

    if (obj != null) {
        obj.className = "";

        var txt = obj.value;
        var rv = txt;

        if (textIsEmpty(txt)) {
            if (canEmpty) return "";

            else {
                obj.className = "errorShadow";
                errors = errors + spacje + emptyError + "<br/>";
                return "";
            }
        }


        if (!ValidateIPaddress(txt)) {
            obj.className = "errorShadow";
            errors = errors + spacje + errorText + "<br/>";
            // rv = "";
        }
        return rv;
    }
    return "";
}


function getVal(ID) {
    var obj = document.getElementById(ID);
    if (obj != null) return obj.value;
    return "";
}


function setVal(ID, val) {
    var obj = document.getElementById(ID);
    if (obj != null) obj.value = val;
    return "";
}

function reload() {
    window.location = "/";
}

function reset() {
	if (inReset) return;
	inReset=true;
var reqReset= new XMLHttpRequest();
    reqReset.open("GET", "reset.html", true);
    reqReset.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    reqReset.onload = function () { 

    }
    reqReset.send(null);
    setTimeout(reload, 10000);
    document.getElementById("res").style.visibility = "hidden";
    document.getElementById("ok1").style.visibility = "hidden";
    document.getElementById("zapis1").innerHTML = "Restartuję. Proszę czekać ....";
}


function percent(nr) {

    if (!isNum(fans_values[nr])) {
        fans_values[nr] = "50";

    }
    var p = parseInt(fans_values[nr]);
    if (p > 100) p = 100;
    if (p < 0) p = 0;
    return p + "%";
}