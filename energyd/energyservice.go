package main

type EnergyService int

type EnergyStatus struct {
	Voltage float32
}

func (t *EnergyService) Status(args *int, reply *EnergyStatus) error {
	status := readControllerStatus()
	*reply = status
	return nil
}
