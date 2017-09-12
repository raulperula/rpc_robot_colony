
module example002()
{
	//intersection() {
		//difference() {
			
			union() {
				cube([80, 76, 3], center = true);
		
translate([0,-37,5])		
cube([80, 2, 10], center = true);
		translate([0,37,5])		
cube([80, 2, 10], center = true);

translate([-20,0,0])
cube([80, 10, 3], center = true);


translate([38.5,0,-10.25])
cube([3, 10, 40.5], center = true);
translate([50,0,-29])
cube([20,10,3],center=true);

difference(){
rotate([0,0,90])
translate([0,39,5])	
cube([76, 2, 10], center = true);

rotate([0,0,90])
translate([0,39,2.5])
cube([10,4,16],center=true);

}
		



		//cube([10, 10, 50], center = true);

			}
		//}
		//translate([0, 0, 5])
		//	cylinder(h = 50, r1 = 20, r2 = 5, center = true);
	//}
}

example002();

