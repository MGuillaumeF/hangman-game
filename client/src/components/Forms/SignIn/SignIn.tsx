import React from 'react'
import { useTranslation } from 'react-i18next';

const formStyle : React.CSSProperties= { marginTop:"1.5em"}
const fieldsetStyle : React.CSSProperties = {display : 'flex', flexDirection : 'column'}

type Props = {
    id : string
}

function SignIn({id}: Props) {
    
  const { t } = useTranslation();
  return (
    <form style={formStyle} method="POST" action="/api/session" id={id}>
      <fieldset style={fieldsetStyle}>
        <legend>{t("forms.sign-in.title")}</legend>
        <label htmlFor="forms.sign-in.fields.identifier">{t("forms.sign-in.fields.identifier.label")}</label>
        <input type="text" name="login" id="forms.sign-in.fields.identifier" required />
        <label htmlFor="forms.sign-in.fields.identifier">{t("forms.sign-in.fields.password.label")}</label>
        <input type="password" name="password" id="forms.sign-in.fields.password" required/>
        <input type="submit" value={String(t("forms.sign-in.fields.submit.label"))} />
      </fieldset>
    </form>
  )
}

export default SignIn