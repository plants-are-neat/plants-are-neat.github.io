function Sensor(x, y, isGateway) {
    this.x = x;
    this.y = y;
    this.isGateway = isGateway;
    this.range = 300;
    this.connected = [];
    this.color = color(0, 255, 0);

    this.show = function() {
        noStroke();
        if (isGateway) fill(255, 255, 0);
        else fill(0, 255, 0)
        ellipse(x, y, 32, 32);

        fill(0, 0, 255, 50);
        ellipse(x, y, this.range / 2, this.range / 2);
    }

    this.inRange = function(other) {
        let dist = sqrt((this.x - other.x) * (this.x - other.x) + (this.y - other.y) * (this.y - other.y));
        return (dist < this.range / 2);
    }

    this.connect = function(other) {
        this.connected.push(other);
        other.connected.push(this);
        this.connected = [...new Set(this.connected)];
        other.connected = [...new Set(other.connected)];
    }


}
