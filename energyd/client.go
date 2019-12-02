package main

import "encoding/json"
import "fmt"
import "net/rpc"

func client() {
	conn, err := rpc.Dial("unix", SockAddr)
	checkError(err)
	defer conn.Close()
	var reply EnergyStatus
	err = conn.Call("EnergyService.Status", 1, &reply)
	json, err := json.Marshal(reply)
	checkError(err)
	fmt.Println(string(json))
}
