package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"net/http"
	"os"
	"strconv"
	"io/ioutil"
	_ "os"
	"time"
)

func main() {
	router := gin.Default()

	// Query string parameters are parsed using the existing underlying request object.
	// The request responds to a url matching:  /welcome?firstname=Jane&lastname=Doe
	router.GET("/welcome", func(c *gin.Context) {
		firstname := c.DefaultQuery("firstname", "Guest")
		lastname := c.Query("lastname") // shortcut for c.Request.URL.Query().Get("lastname")

		c.String(http.StatusOK, "Hello %s %s", firstname, lastname)
	})
	//router.GET("/motion", getSrcText_Id)
	router.GET("/src/text/:id", getSrcText_Id)
	router.GET("/src/photo/:id/:nu", getSrcPhoto_Id_Nu)
	router.GET("/src/voice/:id", getSrcVoice_Id)
	router.GET("/src/accept/:id", getSrcAccept_Id)
	router.Run(":8080")
}

//GET /emotion?skey=&type=&content=&page=&rank=&search=
func getEmotion(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	uid := int64(1)
	

}


//GET /src/text/:id?skey=
func getSrcText_Id(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
		fu
	*/
	id := c.Param("id")

	uid := int64(1)
	txtPath := "./src/" + Itoa64(uid) + "/" + id + "/text"
	text, err := ioutil.ReadFile(txtPath)

	fmt.Printf("\nfile: %s\n", txtPath)
	if err != nil {
		fmt.Printf("\nfail to reach file: %s\nerr = %d", txtPath, err)
		return
	}
	c.String(http.StatusOK, string(text))
}


//GET /src/photo/:id/:nu
func getSrcPhoto_Id_Nu(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	id := c.Param("id")
	num := c.Param("nu")

	uid := int64(1)
	photoPath := "./src/" + Itoa64(uid) + "/" + id + "/photo/" + num
	photo, err := ioutil.ReadFile(photoPath)

	if err != nil {
		fmt.Printf("\nfail to reach photo: %s\nerr = %d", photoPath, err)
		return
	}
	c.Data(http.StatusOK, "image/jpeg", photo)
}

//GET /src/voice/:id?skey=
func getSrcVoice_Id(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	id := c.Param("id")

	uid := int64(1)
	voicePath := "./src/" + Itoa64(uid) + "/" + id + "/voice"
	voice, err := ioutil.ReadFile(voicePath)

	if err != nil {
		fmt.Printf("\nfail to reach voice: %s\nerr = %d", voicePath, err)
		return
	}
	c.Data(http.StatusOK, "audio/mp3", voice)
}

//GET /src/accept/:id?skey=
func getSrcAccept_Id(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	id := c.Param("id")

	uid := int64(1)
	voicePath := "./src/" + Itoa64(uid) + "/" + id + "/accept"
	accept := readFile(voicePath)
	c.
	c.String(http.StatusOK, string(accept))
}

//转换64位非负int的Itoa函数, 方法有点绕，待改
func Itoa64(num int64) string {
	if num == 0 {return "0"}
	str := ""
	for ;num > 0; {
		str += strconv.Itoa(int(num % 10))
		num /= 10
	}
	return str
}

func quickResp(cmd string, c *gin.Context){
	if cmd == OK{
		c.JSON(200, gin.H{
			"msg": "ok",
			"retc": 1,
		})
	} else if cmd == ServerError{
		c.JSON(500, gin.H{
			"msg": "server error",
			"retc": -1,
		})
	} else if cmd == TypeError{
		c.JSON(400, gin.H{
			"msg": "format error",
			"retc": -4,
		})
	} else if cmd == SkeyFail{
		c.JSON(403, gin.H{
			"msg": "skey fail",
			"retc": -3,
		})
	} else if cmd == NotExist{
		c.JSON(404, gin.H{
			"msg": "source not exist",
			"retc": -2,
		})
	} else if cmd == WrongLoginInfo{
		c.JSON(403, gin.H{
			"msg": "wrong login info",
			"retc": -3,
		})
	}
}

func readBinaryFile(path string) ([]byte, error) {
	t, err := ioutil.ReadFile(path)
	if err != nil {
		myLog(fmt.Sprintf("ERROR when openning %v \n", path))
	}
	return t, err
}


func myLog(str string)  {
	fmt.Printf("[%v] %v", time.Now().String(), str)
}
