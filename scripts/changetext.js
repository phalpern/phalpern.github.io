/***************************************************************************
 * ChangeText 
 *   Copyright (C) 2001 Thomas Brattli
 *   This script was released at DHTMLCentral.com
 *   Visit for more great scripts!
 *   This may be used and changed freely as long as this msg is intact!
 *   We will also appreciate any links you could give us.
 *
 *   Made by Thomas Brattli
 *
 *   Script date: 08/02/2001 (keep this date to check versions)
 *   Modifications: 6/22/2003 (Pablo Halpern)
 *****************************************************************************/

function lib_bwcheck(){ //Browsercheck (needed)
    this.ver = navigator.appVersion;
    this.agent = navigator.userAgent;
    this.dom = document.getElementById ? true : false;
    this.opera5 = (navigator.userAgent.indexOf("Opera")>-1 &&
		   document.getElementById);
    this.ie5 = (this.ver.indexOf("MSIE 5")>-1 && this.dom && !this.opera5);
    this.ie6 = (this.ver.indexOf("MSIE 6")>-1 && this.dom && !this.opera5);
    this.ie4 = (document.all && !this.dom && !this.opera5);
    this.ie  = this.ie4 || this.ie5 || this.ie6;
    this.mac = this.agent.indexOf("Mac")>-1;
    this.ns6 = (this.dom && parseInt(this.ver) >= 5); 
    this.ns4 = (document.layers && !this.dom);
    this.bw  = (this.ie6 || this.ie5 || this.ie4 || this.ns4 ||
		this.ns6 || this.opera5);
    return this;
}

var bw = lib_bwcheck();

if(document.layers)  //NS4 resize fix...
{
    scrX= innerWidth; scrY= innerHeight;
    onresize = function()
    {
	if (scrX != innerWidth || scrY != innerHeight) { history.go(0) }
    }
}

// Constructor for change text object
function changeTextObj(msgElementID, cssClass)
{
    this.id = msgElementID;

    if (cssClass)
	this.cssClass = cssClass;
    else
	// Default message style class is ctMessage
	this.cssClass = 'ctMessage';

    this.writeref = null;
    this.css = null;

    // Method implementations differ depending on browser version
    if (bw.dom)
    {
	// For modern browsers supporting W3C DOM
	// Initialization function. We cannot fully initialize at construction
	// because construction may occure before the page fully loads.
	this.initialize = function()
	{
	    this.writeref = document.getElementById(this.id);
	    if (this.writeref)
		this.css = this.writeref.style;
	}

	// Function to actually change the text
	this.writeIt = function(text)
	{
	    this.writeref.innerHTML = text;
	}
    }
    else if (bw.ie4)
    {
	// For MS Internet Explorer 4

	// Initialization function. We cannot fully initialize at construction
	// because construction may occure before the page fully loads.
	this.initialize = function()
	{
	    this.writeref = document.all[this.id];
	    if (this.writeref)
		this.css = this.writeref.style;
	}

	// Function to actually change the text
	this.writeIt = function(text)
	{
	    this.writeref.innerHTML = text;
	}
    }
    else if (bw.ns4)
    {
	// For Netscape Navigator 4

	// Initialization function. We cannot fully initialize at construction
	// because construction may occure before the page fully loads.
	this.initialize = function()
	{
	    this.css = document.layers[this.id];
	    if (this.css)
	        this.writeref = this.css.document;
	}

	// Function to actually change the text
	this.writeIt = function(text)
	{
  	    this.writeref.write(text);
	    this.writeref.close();
	}
    }
    else
    {
	// Browser is not capable of handling change text.
	this.initialize = function() { return this; }
	this.writeIt = function(text) { }
    }

    // Function to actually set the text
    this.changeText = function(newtext, cssClass)
    {
	if (this.css == null) this.initialize();
	if (this.css == null) return;

	if (cssClass == null)
	    cssClass = this.cssClass;

	this.css.visibility = 'visible';
	this.writeIt('<DIV CLASS="' + cssClass + '">' + newtext + '</DIV>');
    }

    return this;
}

/*
</script>
</head>
<body>
<div id="divLinks">
<a href="#" onmouseover="changeText(divMessage, messages[1])">Link 1</a>
<a href="#" onmouseover="changeText(divMessage, messages[2])">Link 2</a>
<a href="#" onmouseover="changeText(divMessage, messages[3])">Link 3</a>
<a href="#" onmouseover="changeText(divMessage, messages[4])">Link 4</a>
<a href="#" onmouseover="changeText(divMessage, messages[5])">Link 5</a>
</div>

<div id="divMessage">
<font face="arial,helvetica,sans-serif" size="2" color="#000000">
Just place the default text here (this will show always in 3.x browsers)
You have to place this with tables or something instead of layers for backwards compatibility.
</font>
</div>

		
</body>
</html>
*/
