import ajax  from './ajax';

import {OtpURL, URL} from './URL'

export const reqLogin = (user) => ajax(URL + 'member/login/',user,
									"","post")

export const reqRegister = (user) => ajax(URL + 'member/register/',user,
									"","post")

export const reqCheckMobile = (user, Token) => ajax(URL + 'member/check/',user,Token,
									"post")

export const reqGetcode = (data) => ajax(OtpURL + 'getcode/65'+data)

export const sendvoucherdata = (data,Token) => ajax(URL + 'voucher/',
									data,Token,"post")

export const sendinvicodedata = (data,Token) => ajax(URL + 'invicode/',
									data,Token,"post")

export const deletevoucherdata = (data,Token) => ajax(URL + 'voucher/'+data+'/',
									'',Token,"delete")

export const patchvoucherdata = (data,Token) => ajax(URL + 'voucher/'+data.id+'/',
									data,Token,"patch")

export const sendmembership = (data,Token) => ajax(URL + 'membership/',
									data,Token,"post")

export const sendoccupation = (data,Token) => ajax(URL + 'campaign/occupation/',
									data,Token,"post")

export const deleteoccupation = (data,Token) => ajax(URL + 'campaign/occupation/'+data+'/',
									'',Token,"delete")

export const reqoccupation = (data,Token) => ajax(URL + 'campaign/occupation/',
									data,Token,"get")

export const requnioccupation = (data,Token) => ajax(URL + 'campaign/occupation/'+data+'/',
									data,Token,"get")

export const sendCampaign = (data,Token) => ajax(URL + 'campaign/',
									data,Token,"post")

export const sendCampaignCondition = (data,Token) => ajax(URL + 'campaign/campaign-condition/',
									data,Token,"post")

export const sendCampaignType = (data,Token) => ajax(URL + 'campaign/campaign-type/ ',
									data,Token,"post")


export const patchCampaign = (data,Token) => ajax(URL + 'campaign/'+data.id+'/',
									data,Token,"patch")

export const patchCampaignCondition = (data,Token,id) => ajax(URL + 'campaign/campaign-condition/'+id+'/',
									data,Token,"patch")

export const patchCampaignType = (data,Token,id) => ajax(URL + 'campaign/campaign-type/'+id+'/',
									data,Token,"patch")

export const deleteCampaign = (data,Token) => ajax(URL + 'campaign/'+data+'/',
									'',Token,"delete")

export const putmembership = (data,Token) => ajax(URL + 'membership/',
									data,Token,"put")

export const patchmembership = (data,Token) => ajax(URL + 'membership/'+data.id+'/',
									data,Token,"patch")

export const deletemembership = (data,Token) => ajax(URL + 'membership/'+data+'/',
									'',Token,"delete")

export const postmembership = (data,Token) => ajax(URL + 'membership/',
									data,Token,"post")

export const reqMember = (Token) => ajax(URL + 'member/',
									{},Token,"get")

export const reqwallet = (Token) => ajax(URL + 'wallet/',
									{},Token,"get")

export const requniwallet = (data,Token) => ajax(URL + 'wallet/',
									data,Token,"get")

export const reqoutlet = (Token) => ajax(URL + 'outlet/',
									{},Token,"get")

export const reqoutletName = (Token,data) => ajax(URL + 'outlet/outlet-name/',
									data,Token,"get")

export const requnioutletName = (data,Token) => ajax(URL + 'outlet/outlet-name/'+data+'/',
									{},Token,"get")

export const reqoutletbydata = (data,Token) => ajax(URL + 'outlet/',
									data,Token,"get")

export const reqproduct = (Token) => ajax(URL + 'product/',
									{},Token,"get")

export const reqUniproductname = (data,Token) => ajax(URL + 'product/'+data+'/',
									{},Token,"get")


export const requniproductname = (data,Token) => ajax(URL + 'product/',data,Token,"get")

export const reqproductsale = (data,Token) => ajax(URL + 'transaction/sales_record/',
									data,Token,"get")

export const requnicategory = (data,Token) => ajax(URL + 'product/category/'+data+'/',
									{},Token,"get")

export const reqcategory = (Token,data) => ajax(URL + 'product/category/',
									data,Token,"get")

export const reqTeaBase = (Token) => ajax(URL + 'product/tea-base/',
									{},Token,"get")

export const requniproduct = (data,Token) => ajax(URL + 'transaction/transact-details/',
									data,Token,"get")

export const requniproduct2 = (data,Token) => ajax(URL + 'transaction/sales_record/',
									data,Token,"get")

export const reqmembership = (Token) => ajax(URL + 'membership/',
									'',Token,"get")

export const reqdatamembership = (data,Token) => ajax(URL + 'membership/',
									data,Token,"get")

export const requnimembership = (data,Token) => ajax(URL + 'membership/'+data+'/',
									'',Token,"get")

export const reqmember = (data,Token) => ajax(URL + 'member/'+data+'/',
									{},Token,"get")

export const requinmember = (data,Token) => ajax(URL + 'member/',
									data,Token,"get")

export const reqtransaction = (data,Token) => ajax(URL + 'transaction/transact-details/',
									data,Token,"get")

export const reqvoucher = (Token) => ajax(URL + 'voucher/',
									{},Token,"get")

export const reqUnivoucher = (data,Token) => ajax(URL + 'voucher/'+data+'/',
									{},Token,"get")

export const requnivoucher = (data,Token) => ajax(URL + 'voucher/',
									data,Token,"get")

export const reqoutletvoucher = (data,Token) => ajax(URL + 'invicode/check/',
									data,Token,"get")

export const reqcoupon = (data,Token) => ajax(URL + 'voucher/'+data+'/',
									{},Token,"get")

export const reqinvicode = (data, Token) => ajax(URL + 'invicode/', data, Token, "get")

export const reqmobileinvicode = (data, Token) => ajax(URL + 'invicode/mobile/', data, Token, "get")

export const reqInvicodeRegister = (data, Token) => ajax(URL + 'invicode/mobile/register/', data, Token, "post")
									
export const reqcampaign  = (Token) => ajax(URL + 'campaign/',
									{},Token,"get")

export const reqcampaignbyID  = (data,Token) => ajax(URL + 'campaign/'+data+'/',
									{},Token,"get")

export const requnicampaignbyid = (data,Token) => ajax(URL + 'campaign/'+data+'/',
									'',Token,"get")

export const requnicampaign = (data,Token) => ajax(URL + 'campaign/',
									data,Token,"get")

export const reqStorebyID = (data,Token) => ajax(URL + 'campaign/performance-outlet/',
									data,Token,"get")

export const reqPerformanceProductbyID = (data,Token) => ajax(URL + 'campaign/performance-product/',
									data,Token,"get")

export const reqtoppings  = (Token,data) => ajax(URL + 'product/toppings/',
									data,Token,"get")

export const requnitoppings  = (data,Token) => ajax(URL + 'product/toppings/'+data+'/',
									{},Token,"get")

export const reqDistrict  = (Token) => ajax(URL + 'outlet/outlet-district/',
									{},Token,"get")

