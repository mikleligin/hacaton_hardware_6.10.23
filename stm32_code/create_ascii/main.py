import PIL.Image
import os.path
import os

path = r'dir'

def rename_files():

    directory = os.fsencode(path)
    cnt = 0
    for file in os.listdir(directory):
        cnt+=1
        filename = os.fsdecode(file)
        pth = path+f"\{filename}"
        os.rename(pth, path+f"\q{cnt}.jpg")
        print(filename)

num_files = len([f for f in os.listdir(path)
                if os.path.isfile(os.path.join(path, f))])
nums = ["1", "1", "1", "0", "0", "0", "0", "0", "0", "0", "0"]
def resize(image, new_width = 64):
    width, heigth = image.size
    ratio = heigth/width
    new_h = int(new_width*ratio)
    resized = image.resize((new_width,64))
    return(resized)
def gray(image):
    gray = image.convert("L")
    return gray
def pixels_to(image):
    pix = image.getdata()
    characters = "".join([nums[pixel//25] for pixel in pix])
    return (characters)
def main():
    sk = '{'
    b = ''
    #print(num_files//12)
    # for j in range(num_files//12+1):
    for j in range(2):
        
        b += f'char p{j+1} [{12}][4097]= {sk}'
        for i in range(12):
            try:
                image = PIL.Image.open(f'dir\q{(i)+12*j+1}.jpg')
            except:
                b+='"\n'
                break
            print((i)+12*j+1, i, j)
            b+='"'
            resize(image)
            print("go")
            print(resize(image))
            new = pixels_to(gray(resize(image)))
            pixel_count = len(new)
            ascii_image = "".join([new[index:(index+64)] for index in range(0, pixel_count, 64)])
            b +=ascii_image
            
            if i*j+1==num_files:
                b+='"\n'
                break
            b+='",\n'
            #print(num_files)
        
        b+="};\n"
        #print(b)
    with open("img.txt", "w") as f:
            f.write(b)
if __name__ == '__main__':
    #main()
    rename_files()
    main()
    