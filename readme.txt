osmMapper

osmMapper is a project I'm using as a task to explore a couple of features.

1. I wanted to explore Cairo as a graphics library. Herb Sutter has mentioned[1] that a graphical library should be
part of the newer C++17 standard and that Cairo was a good fit to use. I thought it would be interesting to explore
it and this project gives me a platform to do that.

2. I was curious as to how well the Boost libraries directed graph and search implementations would fit a real-world
application. Again this project seemed like a good fit to explore this.

So this is mostly an exploratory project, full of experimentation and not necessarily best-practive code.

I've tested it on Mint, built with Clang, CMake, Cairo, Cairomm, PugiXML and X.

Don't muck with the CMake stuff unless you've tested that it doesn't interfere with CLion, as that's what I'm using
as an IDE.

Feel free to fork as you fancy.

Joe

[1] - http://lists.cairographics.org/archives/cairo/2013-December/024858.html
