// ReoScript v1.0 (LGPLv3)
//
// copyright(c) unvell 2013 all rights reserved.
//

function Car(model, color) {
	this.model = model;
	this.color = color;
	this.speed = 0;

	this.speedUp = function() {
		this.speed += 10;
		console.log('current speed is: ' + this.speed);
	};
};

var gol = new Car('Golf 1.6', 'Red');

function run() {

	if (gol.speed < 160) {
		gol.speedUp();
		setTimeout(run, 100);
	} else {
		console.log('Crash!');
	}
}

// let's go!
run();
