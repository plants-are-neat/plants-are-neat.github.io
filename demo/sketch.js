let gateway;
let nodes = [];
let count;
let desiredNode = undefined;
let visited = [];

function setup() {
    createCanvas(600, 600);
    background(52);
    count = 0;
    frameRate(1);
}

function draw() {
    communicate(desiredNode);
}

function communicate(destination) {
    if (destination) {
        let temp = [];
        temp.push(gateway);
        for (let i = 0; i < visited.length; i++) {
            for (let j = 0; j < nodes.length; j++) {
                if (nodes[j].inRange(visited[i])) {
                    temp.push(nodes[j]);
                    temp = [...new Set(temp)];
                    drawLine(nodes[j].x, nodes[j].y, visited[i].x, visited[i].y, color(255,0,0));
                }
            }
        }
        visited = visited.concat(temp);
        visited = [...new Set(visited)];
        for (let i = 0; i < visited.length; i++) {
            if (visited[i].ID == destination) {
                console.log("Gotem");
                noLoop();
                break;
            }
        }
    }
}

function keyPressed() {
    if (key == 'D') {
        for (let i = 0; i < nodes.length; i++) {
            if (abs(nodes[i].x - mouseX) < 32/2 && abs(nodes[i].y - mouseY) < 32/2 && nodes[i] != gateway) {
                nodes.splice(i, 1);
                break;
            }
        }
        reset();
    }
}

function mousePressed() {
    for (let i = 0; i < nodes.length; i++) {
        if (abs(nodes[i].x - mouseX) < 32/2 && abs(nodes[i].y - mouseY) < 32/2) {
            nodes[i].isDesired();
            return;
        }
    }
    if (count == 0) {
        gateway = new Sensor(mouseX, mouseY, true, count);
        nodes.push(gateway);
        visited.push(gateway);
    } else {
        nodes.push(new Sensor(mouseX, mouseY, false, count));
    }
    count++;
    reset();
}

function reset() {
    background(52);

    visited = [];
    desiredNode = undefined;

    gateway.show();
    for (let i = 0; i < nodes.length; i++) {
        nodes[i].show();
    }

    for (let i = 0; i < nodes.length; i++) {
        if (gateway.inRange(nodes[i])) {
            drawLine(gateway.x, gateway.y, nodes[i].x, nodes[i].y, color(255, 255, 255));
        }
    }

    for (let i = 0; i < nodes.length; i++) {
        for (let j = 0; j < nodes.length; j++) {
            if (nodes[i].inRange(nodes[j]) && nodes[i] != nodes[j]) {
                drawLine(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y, color(255, 255, 255));
            }
        }
    }
}

function resetCount() {
    count = 0;
}

function drawLine(x1, y1, x2, y2, col) {
    stroke(col);
    strokeWeight(4);
    line(x1, y1, x2, y2);
}
