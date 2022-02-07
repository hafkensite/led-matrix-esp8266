dist = 10;   // 1cm spacing
ledsize = 6; // 5050 led
height = 5;  // Height of the entire thing
w = 16;
h = 16;
extr_width=0.2; // Nossle width
border = 2.5;

// Calculations
straight_h = height*.7;
angle_h = height - straight_h;
echo("Angled bit height", angle_h);
angle_w = (dist - ledsize)/2;
edge_len = sqrt(angle_h*angle_h + angle_w*angle_w)+0.4;
angle_a = atan(angle_w / angle_h  );


intersection() {
    translate([-border,-border,0])
    cube([w*dist+border*2, h*dist+border*2, height]);
    
    union()
    {
        for (x = [0:h]) {
            translate([dist*w/2, x*dist, height-straight_h/2])
            cube([w*dist, extr_width, straight_h],center=true);
            
            translate([dist*w/2, x*dist+angle_w/2, angle_h/2-0.05])
            rotate([angle_a,0,0])
            cube([w*dist, extr_width, edge_len],center=true);
            
            translate([dist*w/2, x*dist-angle_w/2, angle_h/2-0.05])
            rotate([-angle_a,0,0])
            cube([w*dist, extr_width, edge_len],center=true);
        }
       for (y = [0:w]) {
            translate([y*dist, dist*h/2, height-straight_h/2])
            cube([extr_width, h*dist, straight_h],center=true);

            translate([y*dist+angle_w/2, dist*h/2, angle_h/2-0.05])
            rotate([0,-angle_a,0])
            cube([extr_width, h*dist, edge_len],center=true);
            
            translate([y*dist-angle_w/2, dist*h/2, angle_h/2-0.05])
            rotate([0,angle_a,0])
            cube([extr_width, h*dist, edge_len],center=true);
        }
        // border
        translate([-border, -border,0])
        cube([border, h*dist+border*2, height]);
        translate([w*dist, -border,0])
        cube([border, h*dist+border*2, height]);
        translate([-border, -border,0])
        cube([w*dist+border*2, border, height]);
        translate([-border, h*dist,0])
        cube([w*dist+border*2, border, height]);
    }
}
