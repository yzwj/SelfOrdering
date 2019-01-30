
/* eslint-disable */
import React from 'react'
import {reqGetcode, reqRegister, reqCheckMobile} from '../../api/requrl'
import { createForm } from 'rc-form';
import { Flex, List, InputItem, Toast, Button, WingBlank, WhiteSpace, NavBar, Icon } from 'antd-mobile';

import '../config.js'
import cobra_logo from '../../assets/img/logo.png'
import ReCAPTCHA from 'react-google-recaptcha'
import { flatten } from 'zent/lib/sku/utils';

const Token = 'Token ' + '62c366bacc6365c035fad818d2a77be51097e95d'
class Register extends React.Component {
  state = {
    username: '',
    password: '',
    mobile_no: '',
    invicode: '',
    vericode: '',
    vericode_local: '',
    verified: false,
    count: 60, // 秒数初始化为60秒
    liked: true,
    veri_input: true
  }

  getVerifyCode = async () =>{

    this.setState(
    {
      liked: false
    }
    )
    let { mobile_no,count,liked } = this.state
    const timer = setInterval(() => {
      this.setState({ 
     count: (count--)
     }, () => {
     if (count === 0) {
         clearInterval(timer);
         this.setState({
           liked: true ,
           count: 60
         })
       }
      });
    }, 1000);
    if (mobile_no.length == 8)
    {
      reqCheckMobile({'mobile_no':mobile_no}, Token).then(res => {
        console.log("SUCCESS")
        reqGetcode(mobile_no).then(res => {
          this.setState({
            vericode_local:res.data.message
          })
        })
      }).catch(err => {
        return Toast.info('Mobile number has also exist.')
      })
    }else{
      return Toast.info('Mobile number error')
    }
  }

  handleRegister = async () => {
    let { username, password, mobile_no, invicode, vericode, vericode_local, verified } = this.state
    if (!username || !password) {
      return Toast.info('Please input password.')
    }
    if (vericode != vericode_local){
      return Toast.info('Verify code error!')
    }else{
      this.setState({
        verified:true
      })
    }
    const user = {
      username,
      mobile_no,
      password,
      invicode
    }
    let req2 = reqRegister(user).then(res => {
      username = user.username
      mobile_no = user.mobile_no
      invicode = user.invicode
    }).catch(err => {
      this.setState({
        message: 'Unknown Error'
      })
    })
    this.props.history.push("/login");
  }

  render() {
    const {
      username,
      mobile_no,
      password,
      invicode,
      vericode,
    } = this.state

    return (
      <div>
        <NavBar
          mode="light"
          icon={<Icon type="left" />}
          onLeftClick={() => this.props.history.goBack()}
          rightContent={[
            <Icon key="1" type="ellipsis" />,
          ]}
        >Register</NavBar>
      <form className="form">
        <List renderHeader={() => <b>Register</b>}>
          <Flex justify="center">
            <img src={cobra_logo} height="111" width="256"></img>
          </Flex>
          <WhiteSpace size="lg" />
          <ReCAPTCHA
            sitekey="6LfxbY0UAAAAAHttgY_gbVs1Fj9hhfwTzzt19MA2"
            onChange={(v) => this.setState({ 'veri_input': false})}
            size="normal"
          />
          <WingBlank><InputItem type="tel" maxLength={8} placeholder="Mobile Number" onChange={(v) => this.setState({ 'mobile_no': v })}
            extra={
              this.state.liked
              ?
              <Button inline size="small" className="Getcode" type="ghost" disable={this.state.veri_input} onClick={this.getVerifyCode}>OTP</Button>
              :
              <Button inline size="small" loading disabled className="Getcode" onClick={this.getVerifyCode}>{this.state.count + 's'}</Button>
            }
          required /></WingBlank>
          <WhiteSpace size="lg" />
          <WingBlank>
            <InputItem 
              maxLength={6} 
              placeholder="Verify Code" 
              onChange={(v) => this.setState({ 'vericode': v })} 
              disabled={this.state.veri_input}
              required 
              />
            </WingBlank>
          <WhiteSpace size="lg" />
          <WhiteSpace size="lg" />
          <WingBlank><InputItem maxLength={8} placeholder="User Name" onChange={(v) => this.setState({ 'username': v })} required /></WingBlank>
          <WhiteSpace size="lg" />
          <WingBlank><InputItem type="password" maxLength={16} placeholder="Password" onChange={(v) => this.setState({ 'password': v })} required /></WingBlank>
          <WhiteSpace size="lg" />
          <WingBlank><Button className="Register" type='primary' onClick={this.handleRegister}>Register</Button></WingBlank>
          <WhiteSpace size="xl" />
          <WhiteSpace size="xl" />
        </List>
      </form>
      </div>
    )
  }
}

export default createForm()(Register);
