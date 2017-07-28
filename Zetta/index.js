var zetta = require('zetta');

zetta()
		.name('Hardware Hub LG01')
		.use('lighDriver')
		//.use('scout') //automating device discovering. not implemented yet
		.listen(1337, function(){
				console.log('Zetta server is running at http;//127.0.0.1:1337');
		});
