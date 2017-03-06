let w = 320

let pixel_to_bytes n = n/2
let pixel_to_bytes n = n
let pixel_to_bytes n = n * 2

let rec idx letters c =
  match c with 
    | 'a'..'z' -> idx letters (Char.uppercase c)
    | c ->
	let i = ref 0 in
	let n = String.length letters in
	let found = ref false in
	  while !i < n && not !found do
	    if letters.[!i] = c then found := true
	    else incr i
	  done;
	  if not !found then idx letters ' '
	  else !i

let get_offset i wl hl =
  let by_line = w / wl in
  let lin = i mod by_line in
  let col = i / by_line in
    col * hl * (pixel_to_bytes w) + lin * (pixel_to_bytes wl)

let gen_offset name letters wl hl = 
  print_string "\t.even";print_newline();
  print_string name;print_string ":";print_newline();
  for i = 0 to 255 do
    let c = Char.chr i in
      if i mod 8 = 0 then print_string "\tdc.l\t";
      print_int (get_offset (idx letters c) wl hl);
      if i mod 8 = 7 then print_newline ()
      else print_string ", "
  done;
  print_newline()

let fonte_map = 
  [
    "stabfnt",(32,32,"ABCDEFGHIJKLMNOPQRSTUVWXYZ!.:?()',0123456789+*/#$ ");
    "ballfnt",(32,29,"ABCDEFGHIJKLMNOPQRSTUVWXYZ.!?(),'*/\"$£%0123456789 ");
    "bigfnt",(32,16,"!'(),.0123456789:;? ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    "fnt16x16",(16,16,"!\"£$%&'()*+,-./0123456789:;<=>?%ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
    "fnt1",(32,32,"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789? ");
    "fnt32x32",(32,32,"ABCDEFGHIJKLMNOPQRSTUVWXYZ!?'.0123456789 ");
    "fnt40x40",(40,40,"ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789 ");
    "fontes5",(32,29,"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!?\" ");
    "madfnt",(32,32,"ABCDEFGHIJKLMNOPQRSTUVWXYZ.;\"!?-()$£0123456789*/% ");
    "marbre2",(32,29,"ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
    "megatizer",(32,32,"ABCDEFGHIJKLMNOPQRSTUVWXYZ!?'()123456789,.:*/\"#-$ ");
  ]

let _ = 
  let name = "stabfnt" in
  let (wl,hl,letters) = List.assoc name fonte_map in
    gen_offset name letters wl hl
