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

let w = 64
let h = 64

let _ =
  let hw = (float_of_int (width - w)) /. 2.
  and hh = (float_of_int (height - h)) /. 2.
  in
    for i = 0 to 4*512-1 do
      let alpha = rad_of_deg (360. *. (float_of_int (i mod 512)) /. 512.) in
      let x = ref 0 and y = ref 0 in
	begin
	  match i / 512 with
	    | 0 ->
		x := round ((sin alpha) *. hw +. hw);
		y := round ((cos alpha) *. hh +. hh);
	    | 1 ->
		x := round ((sin (2. *. alpha)) *. hw +. hw);
		y := round ((cos alpha) *. hh +. hh);
	    | 2 ->
		x := round ((sin alpha) *. hw +. hw);
		y := round ((cos (2. *. alpha)) *. hh +. hh);
	    | 3 ->
		x := round ((sin (2. *. alpha)) *. hw +. hw);
		y := round ((cos (2. *. alpha)) *. hh +. hh);
	    | _ -> failwith "error"
	end;
	output_long stdout (!y,!x);
    done
