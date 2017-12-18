(function () {
	"use strict";

	var _px = 0.5; // correcting pixels

	// "consts"
	var BOX_SIZE = 13,
		BOX_MARGIN = 3,
		BOX_SIZE_REAL = BOX_SIZE - BOX_MARGIN,

		TABLE_LEFT = 40,
		TABLE_TOP = 70,

		COUNT_WEEKS = 52,
		COUNT_YEARS = 90,

		CANVAS_WIDTH = 725,
		CANVAS_HEIHT = 1250,

		DEF_BACKGROUND_COLOR = '#FFF',
		DEF_STROKE_COLOR = '#000',
		DEF_FILL_COLOR = '#000',
		DEF_BOX_COLOR = '#FFF',
		PASTDAY_COLOR = '#08F',

		TITLE_TEXT = 'LIFE CALENDAR',
		AXIS_LEFT_TEXT = '← Age',
		AXIS_TOP_TEXT = 'Week of the Year →';

	// canvas
	var c = document.createElement('canvas');
	c.width = CANVAS_WIDTH;
	c.height = CANVAS_HEIHT;
	var ctx = c.getContext('2d');
	ctx.strokeStyle = DEF_STROKE_COLOR;
	ctx.fillStyle = DEF_FILL_COLOR;
	ctx.font = '13px sans-serif';

	// dates
	var currentDate = new Date();
	var bDate = null;

	// public
	window.LC = {
		init: function (element, lang) {
			element.appendChild(c);

			if (!!lang) {
				this.changeLang(lang);
			} else {
				resetCanvas();
				this.update(null);
			}
		},
		update: function (_bDate) {
			bDate = _bDate || new Date();

			drawTable(COUNT_YEARS, COUNT_WEEKS, generateDates());
		},
		changeTheme: function (theme) {

			DEF_STROKE_COLOR = theme.box.borderColor;
			DEF_BOX_COLOR = theme.box.backgroundDefaultColor;
			PASTDAY_COLOR = theme.box.backgroundPastDayColor;

			drawTable(COUNT_YEARS, COUNT_WEEKS, generateDates());
		},
		changeLang: function (lang) {
			TITLE_TEXT = lang.title;
			AXIS_LEFT_TEXT = lang.left_text;
			AXIS_TOP_TEXT = lang.top_text;

			resetCanvas();
			this.update(bDate);
		},

		//show info about the date
		getInfo: function(x, y){
			var _nWeek = 0;
			var _nWeek_of_year = 0;
			var _nYear = 0;
			var _nMonth = 0;
			var rect_size = BOX_SIZE - 1.2;   //regulated in chrome

			y = Math.floor(y) - TABLE_TOP + 10;
			x = Math.floor(x) - TABLE_LEFT + 3;

			if(x > 0 && x < CANVAS_WIDTH &&
			   y > 0 && y < CANVAS_HEIHT
			){
				var xx = Math.floor(x / rect_size) + 1;
				var yy = Math.floor(y / rect_size)
				_nWeek = yy * COUNT_WEEKS + xx;

				if(bDate != null){

					var selected_date = addDays(bDate, _nWeek * 7)

					_nYear = selected_date.getFullYear();
					_nMonth = selected_date.getMonth() + 1;

				}

				var xx = Math.floor(x / rect_size) + 1;
				var yy = Math.floor(y / rect_size)

				return _nYear + '-' + _nMonth  + '<br> No. ' + _nWeek + 'th weeks in life<br>'
				       + yy + ' years old <br>'
			           + xx + ' weeks <br>';

			}
		}
	};


	// CALENDAR

	function   generateDates() {
		var i, j, years = [], weeks;
		for (i = 0; i < COUNT_YEARS; i++) {
			weeks = [];
			for (j = 1; j <= COUNT_WEEKS; j++) {
				var day = getDayObj(i, j);
				weeks.push(day);
			}
			years.push(weeks);
		}
		return years;
	}

	function getDayObj(year, week) {
		var weeks = year * COUNT_WEEKS + week;
		var date = addDays(bDate, weeks * 7);
		return {
			isPast: (currentDate - date) > 0
		};
	}
	function addDays (date, days) {
		var clone = new Date(date.getTime());
		clone.setDate(date.getDate() + days);
		return clone;
	}

	// CANVAS

	function resetCanvas () {
		clearCanvas();
		drawTitle();
		drawAxis();
		drawMetric();
	}

	function clearCanvas () {
		ctx.clearRect(0, 0, CANVAS_WIDTH, CANVAS_HEIHT);
		// draw background:
		ctx.fillStyle = DEF_BACKGROUND_COLOR;
		ctx.fillRect(0, 0, CANVAS_WIDTH, CANVAS_HEIHT);
		ctx.fillStyle = DEF_FILL_COLOR;
	}
	function drawTitle () {
		ctx.textAlign = 'center';
		ctx.fillText(TITLE_TEXT, c.width / 2, 20);
	}
	function drawAxis (textLeft, textTop) {
		ctx.textAlign = 'left';

		// left axis
		ctx.save();
		ctx.translate(20, TABLE_TOP + 40);
		ctx.rotate(-Math.PI/2);
		ctx.translate(-20, -(TABLE_TOP + 40));
		ctx.fillText(AXIS_LEFT_TEXT, 20, TABLE_TOP + 40);
		ctx.restore();

		// top axis
		ctx.fillText(AXIS_TOP_TEXT, TABLE_LEFT, TABLE_TOP - 35);
	}
	function drawMetric () {
		// left metric
		ctx.textAlign = 'right';
		for (var i = 0; i < COUNT_YEARS; i++) {
			if (i % 5 === 0) {
				ctx.fillText(i, TABLE_LEFT - 5, TABLE_TOP + 10 + i * BOX_SIZE);
			}
		}

		// top metric
		ctx.textAlign = 'left';
		for (var i = 1; i < COUNT_WEEKS; i++) {
			if (i % 5 === 0 || i === 1) {
				ctx.fillText(i, TABLE_LEFT + (i - 1) * BOX_SIZE, TABLE_TOP - 10);
			}
		}
	}
	function drawTable (rows, cols, values) {
		for (var i = 0; i < rows; i++) {
			drawRow(i, cols, values[i]);
		}
	}
	function drawRow (row, cols, values) {
		for (var i = 0; i < cols; i++) {
			// color by type
			ctx.fillStyle = values[i].isPast ? PASTDAY_COLOR : DEF_BOX_COLOR;
			ctx.strokeStyle = DEF_STROKE_COLOR;

			drawRect(TABLE_LEFT + i * BOX_SIZE, TABLE_TOP + row * BOX_SIZE);
		}
	}
	function drawRect (x, y) {
		ctx.fillRect(_px + x, _px + y, BOX_SIZE_REAL, BOX_SIZE_REAL);
		ctx.strokeRect(_px + x, _px + y, BOX_SIZE_REAL, BOX_SIZE_REAL);
	}


	//Interaction
	function showInfo(msg){
		var element = document.getElementById("info_for_week");
		if (element != null) {
			element.innerHTML = msg;
		}
		else {
			console.error('Cannot show info');
		}
	}

	function getMousePos(canvas, evt) {
	        var rect = canvas.getBoundingClientRect();
	        return {
	          x: evt.clientX - rect.left,
	          y: evt.clientY - rect.top
	        };
     }

	c.addEventListener('mousemove', function(evt) {
	        var mousePos = getMousePos(c, evt);
	        var msg = LC.getInfo(mousePos.x, mousePos.y);
	        showInfo(msg);
    }, false);

    c.addEventListener('mouseout', function(evt) {
		        var mousePos = getMousePos(c, evt);
		        var msg = LC.getInfo(mousePos.x, mousePos.y);
		        showInfo(msg);
    }, false);


}());