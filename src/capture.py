import cv2
import face_recognition
from PIL import Image

def detect():
    cap = cv2.VideoCapture(0)
    ret, frame = cap.read()
    #grayframe = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.imwrite('test.png', frame)
    cap.release()

    image = face_recognition.load_image_file("face.jpg")

    face_locations = face_recognition.face_locations(image)

    print("I found {} face(s) in this image.".format(len(face_locations)))

    if len(face_locations) > 0:
        return 1
    else:
        return 0

