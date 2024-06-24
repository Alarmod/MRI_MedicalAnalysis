class Protocol:
	ADC = 1
	SWI = 2
	T2 = 3

name2id = dict()
name2id["ep2d_diff_tra_14b"] = Protocol.ADC
name2id["ep2d_diff_tra_14b_1mm"] = Protocol.ADC
name2id["swi_tra"] = Protocol.SWI
name2id["t2_tse_tra_fs"] = Protocol.T2

id2name = dict()
id2name[Protocol.ADC] = "ep2d_diff_tra_14b"
id2name[Protocol.SWI] = "swi_tra"
id2name[Protocol.T2] = "t2_tse_tra_fs"

def get_protocol_id(protocol_name):
	return name2id.get(protocol_name)

def get_protocol_name(protocol_id):
	return id2name.get(protocol_id)
