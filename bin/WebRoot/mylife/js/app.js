/* global selectionDate */
/* global LC */
(function(){

	var DEFAULT_CALENDAR_FILE_NAME = 'Your life calendar',
		DEFAULT_IMAGE_TYPE = 'image/jpeg',

		datePicker,
		theme = document.getElementById('theme'),
		lang = document.getElementById('lang'),
		block = document.getElementById('calendar-canvas'),
		saveImage = document.getElementById('lang_save_jpg'),
		savePdf = document.getElementById('lang_save_pdf'),
		printPage = document.getElementById('lang_print'),

		themes = [
			{
				box: {
					borderColor: '#770',
					backgroundDefaultColor: '#FFF',
					backgroundPastDayColor: '#08F',
				}
			},
			{
				box: {
					borderColor: '#CCC',
					backgroundDefaultColor: '#FFF',
					backgroundPastDayColor: '#87ccc9',
				}
			},
			{
				box: {
					borderColor: '#CCC',
					backgroundDefaultColor: '#FFF',
					backgroundPastDayColor: '#a52126',
				}
			}
		],

		langs = [
			{
				months: [
					'1月',
					'2月',
					'3月',
					'4月',
					'5月',
					'6月',
					'7月',
					'8月',
					'9月',
					'10月',
					'11月',
					'12月'
				],
				settings: {
					lang_set_birthday: "选择出生日期:",
					dp_day: "日",
					dp_month: "月",
					dp_year: "年",
					lang_set_theme: "选择主题:",
					lang_select_lang: "选择语言:",
					lang_save_print: "存储或打印:",
					lang_save_jpg: "JPG",
					lang_save_pdf: "PDF",
					lang_print: "打印",
					lang_info: "信息：",
				},
				calendar: {
					title: '我的4500个星期',
					left_text: '← 年龄',
					top_text: '星期→'
				}
			},
			{
				months: [
					'January',
					'February',
					'March',
					'April',
					'May',
					'June',
					'July',
					'August',
					'September',
					'October',
					'November',
					'December'
				],
				settings: {
					lang_set_birthday: "Set your birthday:",
					dp_day: "Day",
					dp_month: "Month",
					dp_year: "Year",
					lang_set_theme: "Select theme:",
					lang_select_lang: "Select language:",
					lang_save_print: "Save or print:",
					lang_save_jpg: "JPG",
					lang_save_pdf: "PDF",
					lang_print: "Print",
					lang_info: "Information:"
				},
				calendar: {
					title: 'LIFE CALENDAR',
					left_text: '← Age',
					top_text: 'Week of the Year →'
				}
			}
		];


	datePicker = selectionDate(LC.update, langs[0].months);

	updateLang(langs[0].settings);
	LC.init(block, langs[0].calendar);


	theme.onchange = function (e) {
		var themeId = e.target.value || 0;
		var theme = themes[themeId];

		LC.changeTheme(theme);
	};

	lang.onchange = function (e) {
		var langId = e.target.value || 0;
		var lang = langs[langId];

		datePicker = selectionDate(LC.update, lang.months);

		updateLang(lang.settings);
		LC.changeLang(lang.calendar);
	};

	saveImage.onclick = function (e) {
		var image = getImageFromCanvas();
		image = image.replace(DEFAULT_IMAGE_TYPE, 'image/octet-stream');

		e.target.href = image;
		e.target.download = DEFAULT_CALENDAR_FILE_NAME + '.jpg';
	};

	savePdf.onclick = function (e) {
		e.preventDefault();

		var image = getImageFromCanvas(),
			doc = new jsPDF();

		doc.addImage(image, 'JPEG', 20, 5, 170, 280);
		doc.save(DEFAULT_CALENDAR_FILE_NAME + '.pdf');
	};

	printPage.onclick = function (e) {
		e.preventDefault();

		window.print();
	};

	function getImageFromCanvas() {
		return block.querySelector('canvas').toDataURL(DEFAULT_IMAGE_TYPE);
	}

	function updateLang(lang) {
		for (var key in lang)
		{
			if (lang.hasOwnProperty(key)) {
				var element = document.getElementById(key);
				if (element != null) {
					element.textContent = lang[key];
				} else {
					console.error('[updateLang] ' + key);
				}
			}
		}
	}

	var init_page = function(){
		//init with my birthday
		_day = document.getElementById('sdate-day'),
		_month = document.getElementById('sdate-month'),
	    _year = document.getElementById('sdate-year');

		_day.value = 20
		_month.value = 4
	    _year.value = 1978

	    bDate = new Date(_year.value, _month.value, _day.value)
	    LC.update(bDate)

	}

	window.onload = init_page
}());