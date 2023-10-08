from PIL import Image
from colortrans import rgb2short

COLOR_BIT_LENGTH = 8

im = Image.open("images/level_colors.png")
im = im.convert("RGBA")
pix = im.load()


output = []
used = []
colors = {}
width = im.size[0]

used = [[False] * im.size[0] for i in range(im.size[1])]

for i in range(im.size[1]):
  for j in range(im.size[0]):
    if pix[j,i] not in colors:
      if pix[j,i][3] == 0:
        color = -1
      else:
        color = int(rgb2short("{:02x}{:02x}{:02x}".format(pix[j,i][0], pix[j,i][1], pix[j,i][2]))[0])
      
      print(color)
      colors[pix[j,i]] = color

    color = colors[pix[j,i]]

    if not used[i][j]:
      row_count = 0
      col_count = 0
      
      while j + col_count < im.size[0] and pix[j + col_count,i] == pix[j,i] and not used[i][j + col_count]:
        used[i][j + col_count] = True
        col_count += 1
        
      while  i + row_count < im.size[1] and [pix[col,i + row_count] for col in range(j, j+col_count)].count(pix[j,i]) == col_count and (not any(used[i+row_count][j:j+col_count]) or row_count == 0):
        for col in range(j, j+col_count):
          used[i+row_count][col] = True
        row_count += 1
        
      output.append((color, row_count, col_count))


bits = ""
for i in output:
  color = i[0]
  row_count = i[1]
  col_count = i[2]
  
  row_bit_length = row_count.bit_length()
  col_bit_length = col_count.bit_length()
  
  bit_length = max(row_bit_length, col_bit_length) * 2
  if bit_length % 4 != 0:
    bit_length += 4 - bit_length % 4
  
  if color == -1:
    bits += "0"
  else:
    bits += "1"
    
  if row_count == 1 and col_count == 1:
    bits += "000"
  else:
    bits += bin(bit_length // 4)[2:].zfill(3)
  
  if color != -1:
    print(color)
    bits += bin(color)[2:].zfill(COLOR_BIT_LENGTH)
  
  if row_count != 1 or col_count != 1:
    bits += bin(row_count)[2:].zfill(bit_length // 2)
    bits += bin(col_count)[2:].zfill(bit_length // 2)

with open("assets/level_colors.bin", "wb") as f:
  print(bin(width)[2:].zfill(16).encode())
  f.write(width.to_bytes(2, "big"))
  f.write(COLOR_BIT_LENGTH.to_bytes(1, "big"))
  if len(bits) % 8 != 0:
    bits += "0" * (8 - len(bits) % 8)
  for i in range(0, len(bits), 8):
    f.write(bytes([int(bits[i:i+8], 2)]))
  
  
    
  
    