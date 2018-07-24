import cv2
import face_recognition
from PIL import Image

cap = cv2.VideoCapture(0)
ret, frame = cap.read()
#grayframe = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
cv2.imwrite('test.png', frame)
cap.release()

image = face_recognition.load_image_file("test.png")

face_locations = face_recognition.face_locations(image)

print("I found {} face(s) in this image.".format(len(face_locations)))

for face_location in face_locations:
    top, right, bottom, left = face_location

    face_image = image[top:bottom, left:right]
    pil_image = Image.fromarray(face_image)
    pil_image.show()
