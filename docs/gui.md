# Centurion GUI documentation

## gui::Button()
Clickable button with centered text.

### Functions

* constructor
```cpp
Button();
```

* void create():
```cpp
void create(
	std::string imageName,  // button image name (e.g. "button_1")
	std::string Text,       // button text
	int x,                  // x position
	int y,                  // y position
	int pickingID,          // the id to allow click
	glm::vec4 textColor     // button text color
);
```

* void render():
```cpp
void render(
	bool picking   // if true you render with picking color
);
```

Usage example:

```cpp
/* in the .h: */
gui::Button button;

/* in the .cpp: */
// create() function

button = gui::Button();
button.create("button_1", "Text", xpos, ypos, pickingID, glm::vec4(1.f));

// render() function
button.render(true);
picking();  // detect click on the button
button.render(false);
```

## gui::FormInput()
Clickable form with options, background and border.
### Functions
* constructor:
```cpp
FormInput(
	bool options = true
);
```

* void create():
```cpp
void create(
	float x, // x position
	float y, // y position
	float w, // width
	float h, // height
	std::vector<std::string> options, // list of options
	int pickingID // the id to allow click
);
```

* void render():
``` cpp
void render(
	bool picking, // if true you render with picking color
	glm::vec4 color // background color
);
```

* void open_close():
``` cpp
void open_close(); 
``` 
* void close():
``` cpp
void close(); 
``` 
* void select_option():
``` cpp
void select_option(
	int i
);
```

##  gui::Image()
A scalable image.

### Functions

* constructor:

```cpp
Image(
	std::string ImageName // e.g. "main_background"
);
```

* void create():
```cpp
void create(
	std::string Origin, // "top-left", "bottom-left", "center"
	float x, 
	float y, 
	float w, 
	float h, 
	int pickingID
);
```

* void render()
 ```cpp
void render(
	bool picking, 
	float x, // if not specified, it's not read
	float y  // if not specified, it's not read
);
```

##  gui::Rectangle()
A filled clickable rectangle.

### Functions

* constructor:

```cpp
Rectangle()
```

* void create():
```cpp
void create(
	std::string type, // "filled"
	float x, 
	float y, 
	float w, 
	float h, 
	std::string origin, // "top-left", "bottom-left", "center"
	int pickingID
);
```

* void render()
 ```cpp
void render(
	glm::vec4 Color, 
	bool picking = false
);
```

## gui::SimpleText
One-line text.

### Functions

* constructor:

```cpp
SimpleText(
	std::string Type = "static",  // also "dynamic" allowed
	bool Shadow = false
);
```

* void create_static(): more efficient method for static text

```cpp
void create_static(
	std::string text, 
	std::string font, // "tahoma_6", "tahoma_8"
	float x, 
	float y, 
	std::string halign, // "left", "center"
	std::string valign, // "normal", "middle"
	glm::vec4 color
);
```

* void render_static(): more efficient method for static text

```cpp
void render_static();
```

* void render_dynamic(): easier and faster to implement method but less efficient

```cpp
void render_dynamic(
	std::string text, 
	std::string Font,  // "tahoma_6", "tahoma_8"
	float x, 
	float y, 
	glm::vec4 color, 
	std::string halign, // "left", "center"
	std::string valign  // "normal", "middle"
);
```

* float get_width(): returns the width (in pixels) of the entire string
```cpp
float get_width();
```