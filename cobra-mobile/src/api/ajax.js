import axios from 'axios'

export default function ajax(url,data={},
			Token,type='get'){
				
		if(type==='get'){
			let paramStr = ''
			Object.keys(data).forEach(key=>{
				paramStr +=key + '=' + data[key]+'&'
			})
			if(paramStr){
				paramStr = paramStr.substring(0,paramStr.length-1)
			}
			return axios.get(url+'?'+paramStr,{	headers:{ 
								'Content-Type': 'application/json',
								"Authorization": Token, 
			 					}})
		}else if(type==='post'){
			return axios.post(url,data,{	headers:{ 
								'Content-Type': 'application/json',
								"Authorization": Token, 
			 					}})
		}else if(type==='put'){
			return axios.put(url,data,{	headers:{ 
								'Content-Type': 'application/json',
								"Authorization": Token, 
			 					}})
		}else if(type==='patch'){
			return axios.patch(url,data,{	headers:{ 
								'Content-Type': 'application/json',
								"Authorization": Token, 
			 					}})
		}else if(type==='delete'){
			return axios.delete(url,{	headers:{ 
								'Content-Type': 'application/json',
								"Authorization": Token, 
			 					}})
		}
	}
