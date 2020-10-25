function Sensor(x, y, isGateway, ID) {
    this.x = x;
    this.y = y;
    this.isGateway = isGateway;
    this.range = 300;
    this.color = color(0, 255, 0);
    this.ID = ID;

    this.show = function() {
        noStroke();
        if (isGateway) fill(255, 255, 0);
        else fill(0, 255, 0)
        ellipse(this.x, this.y, 32, 32);

        fill(0, 0, 255, 50);
        ellipse(this.x, this.y, this.range / 2, this.range / 2);
    }

    this.inRange = function(other) {
        let dist = sqrt((this.x - other.x) * (this.x - other.x) + (this.y - other.y) * (this.y - other.y));
        return (dist < this.range / 2);
    }

    this.isDesired = function() {
        desiredNode = this.ID;
        noStroke();
        fill(255, 75, 0);
        ellipse(this.x, this.y, 16, 16);
    }


}
