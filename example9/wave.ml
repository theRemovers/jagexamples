let pi = 4. *. (atan 1.)

let rad_of_deg x = (x *. 2. *. pi) /. 360.

let output_word stream v =
  let h = (v lsr 8) land 0xff
  and l = v land 0xff in
    output_byte stream h;
    output_byte stream l

let output_long stream (v1,v2) =
  output_word stream v1;
  output_word stream v2

let output_word_as_long stream v =
  if v < 0 then output_word stream 0xffff
  else output_word stream 0;
  output_word stream v

let round x = int_of_float (x +. 0.5)

let width = 320
let height = 240

let w = 16
let h = 16

let nb = 16

let _ =
  let cx = (float_of_int width) /. 2. 
  and cy = (float_of_int height) /. 2.
  in
  let rmin = 32. in
  let rmax = min ((float_of_int width) -. cx) ((float_of_int height) -. cy) in
    for i = 0 to nb*512-1 do
      let alpha = rad_of_deg (360. *. (float_of_int (i mod 512)) /. 512.) in
      let cosa = cos alpha and sina = sin alpha in
      let rho = ((float_of_int i) *. (rmax -. rmin)) /. (float_of_int (nb * 512)) in
      let rho = rmin +. rho in
      let x = round (cx +. rho *. cosa)
      and y = round (cy +. rho *. sina)
      in
      let x = x - w/2 in
      let y = y - h/2 in
	output_long stdout (y,x);
    done
