// ReoScript
// Import namespaces and classes sample
// 
// copyright (c) 2013 unvell all rights reserved

import System.Windows.Forms.*;
import System.Drawing.*;

var f = new Form() {
  text: 'ReoScript Form', startPosition: 'CenterScreen', 
  resize: function() { this.invalidate(); },
  paint: function(e) { draw(this,e); },
  click: function(e) { console.log(e); },
};

function draw(f,e) {
  var g = e.graphics;
  var p = new Pen(Color.red);
 //g.drawLine(null, 10, 10, 100, 100);
}

f.showDialog();
