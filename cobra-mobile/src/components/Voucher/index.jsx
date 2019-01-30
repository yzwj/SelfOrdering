import React from 'react';
import {
  Tabs, List, WhiteSpace, WingBlank, InputItem, Button, NavBar, Icon, Modal, Toast
} from 'antd-mobile';
import Cookies from 'js-cookie';
import { Dialog, } from 'zent';
import 'zent/css/index.css';
import { Login } from '../Login'
import { reqmobileinvicode, reqInvicodeRegister, requnivoucher, reqoutletvoucher } from '../../api/requrl';
import { QRCode } from 'react-qrcode-logo';
import cobra_logo from '../../assets/img/logo.png'
const { openDialog, closeDialog } = Dialog;
const Item = List.Item;
const Brief = Item.Brief;

function open(code, voucher, outlet) {
  openDialog({
    dialogId: 'my_dialog', // id is used to close the dialog
    title: voucher,
    hasMore: 0,
    maskClosable: true,
    children: <QRCode size={200} value={code} logoImage={cobra_logo} logoWidth={64} logoHeight={27} ecLevel={'H'} />,
    footer: <p align="center">{'Outlets: ' + outlet}</p>,
    style: { 'textAlign': 'center', 'width': '250px'},
    onClose() {
      console.log('outer dialog closed');
    },
  });
};

function open_outletless(code, voucher) {
  openDialog({
    dialogId: 'my_dialog', // id is used to close the dialog
    title: voucher,
    hasMore: 0,
    maskClosable: true,
    children: <QRCode size={200} value={code} logoImage={cobra_logo} logoWidth={64} logoHeight={27} ecLevel={'H'} />,
    footer: <p align="center">{' '}</p>,
    style: { 'textAlign': 'center', 'width': '250px'},
    onClose() {
      console.log('outer dialog closed');
    },
  });
};

export default class Demo extends React.Component {
  state = {
    visible: false,
    confirmLoading: false,
    invicode: [],
    outlets: [],
    membership: [],
    voucherdata: [],
    topping: [],
    initializing: 1,
    hasMore: 0
  }

  // componentDidMount() {
  //   this.changeTitle('User');
  // }


  handleSendInvicode = async () => {
    let { invicode_send } = this.state
    const Token = 'Token ' + Cookies.get('Authorization')
    const user = {
      username,
      invicode_send
    }
    user.username = Cookies.get('user')

    if (user.username){
    reqInvicodeRegister(user, Token).then(res => {
      username = user.username
      invicode = user.invicode_send
    }).catch(err => {
      if (err.response.status == 500){
        Toast.fail('Invicode is invaild!', 1)
      }else{
        Toast.success('Success!', 1);
        this.props.history.goBack();
      }
      this.setState({
        message: 'Unknown Error'
      })
      })
      Toast.success('Success!ß', 1);
      this.props.history.goBack();
      
  }else{
    this.props.history.push('/login')
  }
}

  async componentWillMount() {
    let { invicode, voucherdata } = this.state
    this.setState({
      hasMore: 1,
      initializing: 2, // initialized
    });
    const Token = 'Token ' + Cookies.get('Authorization')
    let req = await reqmobileinvicode({ 'user': Cookies.get('user') }, Token)
    let req2 = await requnivoucher({'page_size': 999}, Token)
    voucherdata.push(req2.data.results)

    invicode = req.data.vouchers
    if (invicode && invicode.length > 0){
      for(let i=0;i<invicode.length;i++){
        const req3 = await reqoutletvoucher({'code':invicode[i][6]}, Token)
        const outlet = req3.data.outlet
        let outlet_r =[]
        if (outlet)
        {
          outlet.map(k=>{
            console.log(k)
            switch (k[2])
            {
              case 7:
              outlet_r.push('201')
              break;
              case 8:
              outlet_r.push('828')
              break;
              case 9:
              outlet_r.push('OTH')
              break;
              case 10:
              outlet_r.push('EM')
              break;
              case 11:
              outlet_r.push('BK')
              break;
              case 12:
              outlet_r.push('Nex')
              break;
              case 13:
              outlet_r.push('PLS')
              break;
              case 14:
              outlet_r.push('TP')
              break;
              case 15:
              outlet_r.push('CL')
              break;
              case 16:
              outlet_r.push('504')
              break;
              case 17:
              outlet_r.push('372')
              break;
              case 18:
              outlet_r.push('YS')
              break;              
              case 19:
              outlet_r.push('888')
              break;
            }
          })
        }
        voucherdata[0].map( j=>{
          if (invicode[i][3] == j.id){
            invicode[i][3] = j.name
            invicode[i].push(j.effective_date)
            invicode[i].push(j.expiring_date)
            invicode[i].push(outlet_r)
          }
        })
      }
    }
    this.setState({voucherdata})
    this.setState({ invicode })
  }
  

  async loadInvicode() {
    let { invicode, voucherdata } = this.state
    const Token = 'Token ' + Cookies.get('Authorization')
    let req = await reqmobileinvicode({ 'user': Cookies.get('user') }, Token)
    let req2 = await requnivoucher({'page_size': 999}, Token)
    voucherdata.push(req2.data.results)

    invicode = req.data.vouchers
    if (invicode && invicode.length > 0){
      for(let i=0;i<invicode.length;i++){
        const req3 = await reqoutletvoucher({'code':invicode[i][6]}, Token)
        const outlet = req3.data.outlet
        let outlet_r = []
        if (outlet)
        {
          outlet.map(k=>{
            switch (k[2])
            {
              case 7:
              outlet_r.push('201')
              break;
              case 8:
              outlet_r.push('828')
              break;
              case 9:
              outlet_r.push('OTH')
              break;
              case 10:
              outlet_r.push('EM')
              break;
              case 11:
              outlet_r.push('BK')
              break;
              case 12:
              outlet_r.push('Nex')
              break;
              case 13:
              outlet_r.push('PLS')
              break;
              case 14:
              outlet_r.push('TP')
              break;
              case 15:
              outlet_r.push('CL')
              break;
              case 16:
              outlet_r.push('504')
              break;
              case 17:
              outlet_r.push('372')
              break;
              case 18:
              outlet_r.push('YS')
              break;        
              case 19:
              outlet_r.push('888')
              break;
            }
          })
        }
        voucherdata[0].map( j=>{
          if (invicode[i][3] == j.id){
            invicode[i][3] = j.name
            invicode[i].push(j.effective_date)
            invicode[i].push(j.expiring_date)
            invicode[i].push(outlet_r)
          }
        })
      }
    }
    this.setState({voucherdata})
    this.setState({ invicode })
  }

  render() {
    let { invicode, invicode_send, voucherdata } = this.state
    var now = new Date();
    return (
      Cookies.get('isLogin') ? (
      <div>
        <NavBar
          mode="light"
          icon={<Icon type="left" />}
          onLeftClick={() => this.props.history.goBack()}
          rightContent={[
            <Icon key="1" type="ellipsis" />,
          ]}
        >Voucher</NavBar>
          
        <Tabs tabs={[{ title: 'VOUCHERS' }, { title: 'REDEEMED' }, { title: 'INVICODE' }]}
          renderTab={tab => tab.title}
          onTabClick={ (tab => this.loadInvicode()) }
        >
          <div style={{ alignItems: 'center', justifyContent: 'center', backgroundColor: '#fff' }}>
            {
              invicode.map((item => {
                if (item[1] != 2) {
                  var dateFormat = require('dateformat');
                  let start_time = new Date(item[9]).getTime();
                  let cur_time = dateFormat(start_time, "mm/dd/yyyy")
                  let stop_time = new Date(item[10]).getTime();
                  let end_time = dateFormat(stop_time, "mm/dd/yyyy")
                  if (stop_time > now.getTime()) {
                    if (item[11] != '')
                    {
                      return (
                        <div>
                        <List renderHeader=' ' className="invicode-list">
                          <Item multipleLine extra='Voucher' onClick={open.bind(this, item[6], item[3], item[11])} style={{ backgroundColor: '#33FF33' }}>
                          {item[3]} <Brief>{cur_time+' ~ '+end_time}</Brief>
                          </Item>
                        </List>
                        </div>
                      )
                    }else{
                      return(
                        <div>
                        <List renderHeader=' ' className="invicode-list">
                          <Item multipleLine extra='Voucher' onClick={open_outletless.bind(this, item[6], item[3])} style={{ backgroundColor: '#33FF33' }}>
                          {item[3]} <Brief>{cur_time+' ~ '+end_time}</Brief>
                          </Item>
                        </List>
                        </div>
                        )    
                    }
                  }}
              }))
            }
          </div>
          <div style={{ alignItems: 'center', justifyContent: 'center', backgroundColor: '#fff' }}>
            {
              invicode.map((item => {
                if (item[1] == 2) {
                  var dateFormat = require('dateformat');
                  let start_time = new Date(item[9]).getTime();
                  let cur_time = dateFormat(start_time, "mm/dd/yyyy")
                  let stop_time = new Date(item[10]).getTime();
                  let end_time = dateFormat(stop_time, "mm/dd/yyyy")
                  return (
                    <List renderHeader=' ' className="invicode-list">
                        <Item multipleLine extra='Voucher' style={{ backgroundColor: '#C0C0C0' }}>
                        {item[3]} <Brief>{cur_time+' ~ '+end_time}</Brief>
                      </Item>
                    </List>
                  )
                }
              }))
            }
          </div>
          <div style={{ alignItems: 'center', justifyContent: 'center', backgroundColor: '#fff' }}>
            <WingBlank><InputItem maxLength={16} placeholder="Invitation Code" onChange={(v) => this.setState({ 'invicode_send': v })} required /></WingBlank>
            <WhiteSpace size="lg" />
            <WingBlank><Button className="Register" type='primary' onClick={this.handleSendInvicode}>Add Invicode or abbreviation</Button></WingBlank>
            <WhiteSpace size="xl" />
          </div>
        </Tabs>
      </div>) : this.props.history.push("/login") );
  }
}
