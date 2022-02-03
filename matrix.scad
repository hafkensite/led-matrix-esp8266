dist = 10;
height = 5; // Height of the entire thing
border = 5;
w = 16;
h = 16;
pad = 0.01;
scale = 0.5;
channel_width = 1.5;
margin = 0.0001;
$fn=1;
difference()
{
    // main body
    translate([ 0, 0, -height / 2 ])
    {
        cube(
            [
                w * dist + border * 2,
                h * dist + border * 2

                ,
                height
            ],
            center = true);
    }
    union()
    {
        // trapeziods
        for (x = [1:w]) {
            for (y = [1:h]) {
                translate([
                    x * dist - ((w + 1) / 2) * dist,
                    y * dist - ((h + 1) / 2) * dist,
                    margin
                ])
                {

                    rotate(a = [ 0, 180, 0 ])
                        linear_extrude(height = height + margin * 2, scale = scale)
                            translate([
                                -dist * (0.5 - pad / 2),
                                -dist * (0.5 - pad / 2),
                                0
                            ]) square(dist * (1 - pad));
                }
            }
        }

        // channels at the bottom
        for (x = [1:w - 1]) {
            polyhedron(points = [[x * dist - channel_width - w * dist / 2,
                                  h * dist / 2,
                                  -height - margin],
                                 [x * dist + channel_width - w * dist / 2,
                                  h * dist / 2,
                                  -height - margin],
                                 [x * dist + channel_width - w * dist / 2,
                                  -h * dist / 2,
                                  -height - margin],
                                 [x * dist - channel_width - w * dist / 2,
                                  -h * dist / 2,
                                  -height - margin],
                                 [x * dist - w * dist / 2,
                                  h * dist / 2 - channel_width,
                                  -2],
                                 [x * dist - w * dist / 2,
                                  -h * dist / 2 + channel_width,
                                  -2]],
                       faces = [
                           [ 3, 2, 1, 0 ], // bottom
                           [ 0, 1, 4 ],    // sides
                           [ 5, 4, 1, 2 ],
                           [ 2, 3, 5 ],
                           [ 5, 3, 0, 4 ]

                       ]);
        }
        for (y = [1:h - 1]) {
            polyhedron(points =
                           [
                               [
                                   -w * dist / 2,
                                   y * dist - channel_width - h * dist / 2,
                                   -height - margin
                               ],
                               [
                                   -w * dist / 2,
                                   y * dist + channel_width - h * dist / 2,
                                   -height - margin
                               ],
                               [
                                   w * dist / 2,
                                   y * dist + channel_width - h * dist / 2,
                                   -height - margin
                               ],
                               [
                                   w * dist / 2,
                                   y * dist - channel_width - h * dist / 2,
                                   -height - margin
                               ],
                               [
                                   -w * dist / 2 + channel_width,
                                   y * dist - h * dist / 2,
                                   -2
                               ],
                               [
                                   w * dist / 2 - channel_width,
                                   y * dist - h * dist / 2,
                                   -2
                               ]
                           ],
                       faces = [
                           [ 3, 2, 1, 0 ], // bottom
                           [ 0, 1, 4 ],    // sides
                           [ 5, 4, 1, 2 ],
                           [ 2, 3, 5 ],
                           [ 5, 3, 0, 4 ]

                       ]);
        }
    }
}