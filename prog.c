
displ (var d) {
    var k;
    out(d & 255);
}




main () {

    while (!0) {
        var g;
        for (g = 0; g != 2; g += 1) {
            var p;
            for (p = (g ? 1 : 128); p; p = (g ? p << 1 : p >> 1)) {
                displ(p);
            }
        }
    }
}


