
/* ReoScript Graphics Library Extension */

var Graphics = {

  Rectangle: function(x, y, width, height) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
  },

  Size: function(width, height) {
    this.width = width;
    this.height = height;
  },

  Point: function(x, y) {
    this.x = x;
    this.y = y;
  },

  Color: function(a, r, g, b) {
    if (__args__.length == 4) {
      this.a = a;
      this.r = r;
      this.g = g;
      this.b = b;
    } else if (__args__.length == 3) {
      this.r = a;
      this.g = r;
      this.b = g;
    } else if (__args__.length == 1 && Graphics.extern.DecodeColor != null) {
      var decodedColor = Graphics.extern.DecodeColor(a);
      this.a = decodedColor.a;
      this.r = decodedColor.r;
      this.g = decodedColor.g;
      this.b = decodedColor.b;
    }
  },

  // common methods
  common: {
    offset: function(x, y) {
      this.x += x;
      this.y += y;
    }
  },

  // Reserve for native code call
  extern: {
  }
};

Graphics.Rectangle.prototype.offset = Graphics.common.offset;

Graphics.Rectangle.prototype.size = function() {
  return new Graphics.Size(this.x, this.y);
};

Graphics.Rectangle.prototype.right = function() {
  return this.x + this.width;
};

Graphics.Rectangle.prototype.bottom = function() {
  return this.y + this.height;
};
  
Graphics.Point.prototype.offset = Graphics.common.offset;


