
/* eslint-disable */
import React from 'react'
import ReactDOM from 'react-dom'
import { reqLogin } from '../../api/requrl'
import { createForm } from 'rc-form';
import { Link } from 'react-router-dom'
import Cookies from 'js-cookie';
import { Flex, List, InputItem, Toast, Button, WingBlank, WhiteSpace, NavBar, Icon } from 'antd-mobile';

import '../config.js'
import cobra_logo from '../../assets/img/logo.png'

class Login extends React.Component {
  state = {
    username: '',
    password: '',
  }

  handleLogin = async () => {
    let { username, password, isLogin } = this.state
    if (!username || !password) {
      return Toast.info('Please input password.')
    }
    const user = {username, password}
    let id=window.location.href.split("?")[1]
    reqLogin(user).then(res => {
      global.isLogin = true
      global.username = user.username
      Cookies.set('isLogin', global.isLogin);
      Cookies.set('user', user.username);
      Cookies.set('Authorization', res.data.token)
      this.props.history.push("/profile")
    }).catch(err => {
      console.log(err.message)
      return Toast.info('Username or password error.')
    })
  }

  render() {
    const {
      username,
      password,
    } = this.state

    return (
      <div style={{cursor:'pointer'}}>
        <head><script src='https://www.google.com/recaptcha/api.js?render=6LcQbI0UAAAAADFfIF-Kdc1WlQJWsH4Ul-7IyBRg'></script></head>
        <NavBar
          mode="light"
          icon={<Icon type="left" />}
          onLeftClick={() => this.props.history.goBack()}
          rightContent={[
            <Icon key="1" type="ellipsis" />,
          ]}
        >Login</NavBar>
      <form className="form">
        <List renderHeader={() => <b>Login</b>}>
          <Flex justify="center">
            <img src={cobra_logo} height="111" width="256"></img>
          </Flex>
          <WhiteSpace size="lg" />
          <WingBlank><InputItem type="tel" placeholder="Mobile Number" onChange={(v) => this.setState({ 'username': v })} required /></WingBlank>
          <WhiteSpace size="lg" />
          <WingBlank><InputItem type="password" placeholder="Password" onChange={(v) => this.setState({ 'password': v })} required /></WingBlank>
          <WhiteSpace size="xl" />
          <WingBlank><Button className="Login" type='primary' style={{cursor:'pointer'}} onClick={this.handleLogin}>Login</Button></WingBlank>
          <WhiteSpace size="xl" />
          <Flex justify="between">
            <WingBlank><li><Link to={"/register"}>Forget password</Link></li></WingBlank>
            <WingBlank><li><Link to={"/register"}>User Register</Link></li></WingBlank>
          </Flex>
          <WhiteSpace size="xl" />
        </List>
      </form>
      </div>
    )
  }
} 

export default createForm()(Login) ;
