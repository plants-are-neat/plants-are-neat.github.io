let gateway;
let nodes = [];
let count;
let desiredNode = undefined;

function setup() {
    createCanvas(600, 600);
    background(52);
    count = 0;
    //for (let i = 0; i < 4; i++) {
    //    nodes[i] = new Sensor(random(width / 4, 3 * width / 4), random(height / 2, height - 64), false);
    //}

    //reset();
}

function draw() {
    communicate(desiredNode);
}

function communicate(destination) {
    if (destination) {
        if (!gateway.connected.includes(destination)) {
            for (let i = 0; i < gateway.connected.length; i++) {
                for (let j = 0; j < gateway.connected[i].connected.length; j++) {
                    if (!gateway.connected.includes(gateway.connected[i].connected[j])) {
                        gateway.connected.push(gateway.connected[i].connected[j]);
                    }
                }
            }
            gateway.connected = [...new Set(gateway.connected)];
        } else {
            console.log("Found");
            reset();
        }
    }
}

function keyPressed() {
    console.log(key);
    if (key == ' ') {
        for (let i = 0; i < nodes.length; i++) {
            if (abs(nodes[i].x - mouseX) < 32/2 && abs(nodes[i].y - mouseY) < 32/2) {
                desiredNode = nodes[i];
                return;
            }
        }
    }
    if (key == 'A') {
        let deleteThis;
        for (let i = 0; i < nodes.length; i++) {
            if (abs(nodes[i].x - mouseX) < 32/2 && abs(nodes[i].y - mouseY) < 32/2) {
                deleteThis = i;
            }
        }
        nodes.splice(deleteThis, 1);
        reset();
    }
}

function mousePressed() {
    if (count == 0) {
        gateway = new Sensor(mouseX, mouseY, true);
    } else {
        nodes.push(new Sensor(mouseX, mouseY, false));
    }
    count++;
    reset();
}

function reset() {
    background(52);

    gateway.connected = [];
    for (let i = 0; i < nodes.length; i++) {
        nodes[i].connected = [];
    }
    desiredNode = undefined;

    gateway.show();
    for (let i = 0; i < nodes.length; i++) {
        nodes[i].show();
    }

    for (let i = 0; i < nodes.length; i++) {
        if (gateway.inRange(nodes[i])) {
            gateway.connect(nodes[i]);
            drawLine(gateway.x, gateway.y, nodes[i].x, nodes[i].y, color(255, 255, 255));
        }
    }

    for (let i = 0; i < nodes.length; i++) {
        for (let j = 0; j < nodes.length; j++) {
            if (nodes[i].inRange(nodes[j]) && nodes[i] != nodes[j]) {
                nodes[i].connect(nodes[j]);
                drawLine(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y, color(255, 255, 255));
            }
        }
    }
}

function drawLine(x1, y1, x2, y2, col) {
    stroke(col);
    strokeWeight(4);
    line(x1, y1, x2, y2);
}
