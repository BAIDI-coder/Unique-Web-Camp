package main

import (
	"encoding/json"
	"fmt"
	"github.com/gin-gonic/gin"
	"github.com/xormplus/xorm"
	_ "github.com/go-sql-driver/mysql"
	"io/ioutil"
	"net/http"
	_ "os"
	_ "sort"
	"strconv"
	"time"
	_ "strings"
)

var (
	OK = "ok"
	ServerError = "serverError"
	FormatError = "formatError"
	SkeyFail = "skeyFail"
	NotExist = "notExist"
	WrongLoginInfo = "wrongLoginInfo"
	NotUploading = "notUploading"
	UploadSuccess = "uploadSuccess"
)

var (
	Sql *xorm.Engine
	router *gin.Engine
)

type emotion struct {
	Id      int64 `json:"id" xorm:pk`
	Uid     int64 `json:"uid"`
	Stars   int64 `json:"stars"`
	Type int64 `json:"type"`
	Brief string `json:"brief" xorm:varchar(100)`
	Content int64 `json:"content"`
	PhotoNum int64 `json:"photoNum"`
	CreatedAt time.Time `json:"createdAt" xorm:"created"`
	Text string `json:"content" xorm:"varchar(2000)"`
	Accept string `json:"content" xorm:"varchar(2000)"`
}

func main() {
	router = gin.Default()
	router.GET("/emotions", getEmotion)
	router.GET("/src/text/:id", getSrcText_Id)
	router.GET("/src/photo/:id/:nu", getSrcPhoto_Id_Nu)
	router.GET("/src/voice/:id", getSrcVoice_Id)
	router.GET("/src/accept/:id", getSrcAccept_Id)
	router.GET("/test", getTest)

	dataSourceName := "root:baiweiliang1234@/test?charset=utf8"
	Sql, err := xorm.NewEngine("mysql", dataSourceName)
	if err != nil {
		myLog("fail to link to db: " + dataSourceName)
		return
	} else {
		myLog("link success!")
	}
	err = Sql.Sync2(new(emotion))
	if err != nil {
		myLog("fail to sync with table: " + "emotion")
		return
	} else {
		myLog("sync success!")
	}
	Sql.ShowSQL(true)	//回显SQL语句

	router.Run(":80")
}

//GET /emotions?skey=&type=&content=&page=&rank=&search=&full=
func getEmotion(c *gin.Context) {
	/*skey := c.Query("skey")
	uid := checkSession(skey)
	if uid == -1 {
		quickResp("SkeyFail", c)
		return
	}*/
	dataSourceName := "root:baiweiliang1234@/test?charset=utf8"
	Sql, err := xorm.NewEngine("mysql", dataSourceName)
	uid := int64(951212)	//Debug
	ty := c.DefaultQuery("type", "-1")	//-1表示忽略
	content := c.DefaultQuery("content", "-1")
	page := c.DefaultQuery("page", "1")	//Defult: p1
	rank := c.DefaultQuery("rank", "0")	//0 no sort;1 time up;2 star up
	search := c.DefaultQuery("search", "")	//""表示忽略
	full := c.DefaultQuery("full", "0")	//0表示不反回text和accept，1表示全部返回

	//填充sql语句: WHERE & ORDER
	where0, where1, where2, where3 := "", "", "", ""
	where0 = "uid=" + Itoa64(uid) + " "
	switch ty {
	case "0": where1 = "AND type=0 "
	case "1": where1 = "AND type=1 "
	default:
		where1 = ""
	}
	if content != "-1" && content != "" {where2 = "AND content=" + content + " "}
	if search != "" {where3 = "AND text LIKE '%" + search + "%'"}
	order := " ORDER BY"
	switch rank {
	case "1": order += " created_at ASC"
	case "-1": order += " created_at DESC"
	case "2": order += " stars ASC"
	case "-2": order += " stars DESC"
	default:
		order = ""
	}
	//分页
	pageNum, _ := strconv.Atoi(page)
	offset := 1
	if pageNum > 0 { offset = (pageNum - 1) * 20 + 1}
	limit := " LIMIT " + strconv.Itoa(offset) + "," + "20"

	//组装sql
	var sql string
	if full == "0" {sql = "SELECT id,stars,type,content,photo_num,brief,created_at FROM emotion WHERE "} else {
		sql = "SELECT id,stars,type,content,photo_num,brief,created_at,text,accept FROM emotion WHERE "
	}
	sql += where0 + where1 + where2 + where3 + order + limit + ";"

	type emotionList struct {
		Stars   int64 `json:"stars"`
		Type int64 `json:"type"`
		Brief string `json:"brief" xorm:varchar(100)`
		Content int64 `json:"content"`
		PhotoNum int64 `json:"photoNum"`
		CreatedAt time.Time `json:"createdAt" xorm:"created"`
	}
	type emotionListAll struct {
		Stars   int64 `json:"stars"`
		Type int64 `json:"type"`
		Brief string `json:"brief" xorm:varchar(100)`
		Content int64 `json:"content"`
		PhotoNum int64 `json:"photoNum"`
		CreatedAt time.Time `json:"createdAt" xorm:"created"`
		Text string `json:"text"`
		Accept string `json:"accept"`
	}

	//根据full值响应相应的json
	if full == "0" {
		list := make([]emotionList, 0)
		err := Sql.Sql(sql).Find(&list)
		if err != nil {
			quickResp(NotExist, c)
			return
		}
		type results struct {
			Page int64	`json:"page"`
			Num int64 `json:"num"`
			EmotionList []emotionList `json:"emotionList"`
		}
		if len(list) == 0 {pageNum = 1}
		respStruct := results{
			Page:        int64(pageNum),
			Num:         int64(len(list)),
			EmotionList: list,
		}
		c.JSON(http.StatusOK, respStruct)
	} else {
		list := make([]emotionListAll, 0)
		err = Sql.Sql(sql).Find(&list)
		if err != nil {
			quickResp(NotExist, c)
			return
		}
		type results struct {
			Page int64	`json:"page"`
			Num int64 `json:"num"`
			EmotionList []emotionListAll `json:"emotionList"`
		}
		if len(list) == 0 {pageNum = 1}
		respStruct := results{
			Page:        int64(pageNum),
			Num:         int64(len(list)),
			EmotionList: list,
		}
		c.JSON(http.StatusOK, respStruct)
	}
	return
}

//GET /src/text/:id?skey=
func getSrcText_Id(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	uid := int64(951212)
	dataSourceName := "root:baiweiliang1234@/test?charset=utf8"
	Sql, err := xorm.NewEngine("mysql", dataSourceName)
	type emotionList struct {
		Text string `json:"content" xorm:"varchar(2000)"`
		Accept string `json:"content" xorm:"varchar(2000)"`
	}
	var results []string
	err = Sql.Sql("SELECT text FROM emotion WHERE uid=?", uid).Find(&results)
	if err != nil {
		quickResp(NotExist, c)
		return
	}
	c.String(http.StatusOK, results[0])
}

//GET /src/photo/:id/:nu
func getSrcPhoto_Id_Nu(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	uid := int64(1)
	id := c.Param("id")
	num := c.Param("nu")

	photoPath := "../src/" + Itoa64(uid) + "/" + id + "/photo/" + num
	photo, err := ioutil.ReadFile(photoPath)

	if err != nil {
		fmt.Printf("\nfail to reach photo: %s\nerr = %d", photoPath, err)
		return
	}
	c.Data(http.StatusOK, "image", photo)
}

//GET /src/voice/:id?skey=
func getSrcVoice_Id(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	uid := int64(1)
	id := c.Param("id")

	voicePath := "../src/" + Itoa64(uid) + "/" + id + "/voice"
	voice, err := ioutil.ReadFile(voicePath)

	if err != nil {
		fmt.Printf("\nfail to reach voice: %s\nerr = %d", voicePath, err)
		return
	}
	c.Data(http.StatusOK, "audio", voice)
}

//GET /src/accept/:id?skey=
func getSrcAccept_Id(c *gin.Context) {
	/*
		skey := c.Query("skey")
		uid := checkSession(skey)
	*/
	uid := int64(951212)
	id := c.DefaultQuery("id", "")
	accept := ""
	err := Sql.Select("accept").Where("uid=?", uid).And("id=?", id).Find(&accept)
	if err != nil {
		quickResp(NotExist, c)
		return
	}
	c.String(http.StatusOK, accept)
}

//转换64位非负int的Itoa函数, 方法有点绕，待改
func Itoa64(num int64) string {
	if num == 0 {return "0"}
	str := ""
	for ;num > 0; {
		str += strconv.Itoa(int(num % 10))
		num /= 10
	}
	strune := []rune(str)
	var ans string
	for l := len(strune) - 1;l >= 0;l-- {
		ans += string(strune[l])
	}
	return ans
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

func getTest(c *gin.Context) {
	type emotionList struct {
		Stars   int64 `json:"stars"`
		Type int64 `json:"type"`
		Brief string `json:"brief" xorm:varchar(100)`
		Content int64 `json:"content"`
		PhotoNum int64 `json:"photoNum"`
		CreatedAt time.Time `json:"createdAt" xorm:"created"`
	}
	var results []emotionList
	sql := "SELECT * FROM emotion WHERE uid=8080 AND text LIKE 'this%' ORDER BY created_at DESC LIMIT 3, 4;"
	err := Sql.Sql(sql).Find(&results)
	if err != nil {
		fmt.Println("fali to quetr")
		quickResp(NotExist, c)
		return
	}
	outJson, err := json.Marshal(results)
	if err != nil {
		fmt.Println("marshal failede")
		quickResp(NotExist, c)
		return
	}
	c.Data(200, "application/json", outJson)
	return
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
	} else if cmd == FormatError{
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
	} else if cmd == NotUploading{
		c.JSON(403, gin.H{
			"msg": "please POST emotion first",
			"retc": -5,
		})
	} else if cmd == UploadSuccess{
		c.JSON(200, gin.H{
			"msg": "upload success",
			"retc": 2,
		})
	}
}