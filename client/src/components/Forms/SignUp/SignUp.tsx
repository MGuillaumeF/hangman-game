import React from "react";
import { useTranslation } from "react-i18next";
import api from "../../../generated/.api_doc_parameters.json";
import BasicInput from "../../BasicInput/BasicInput";
import BasicForm from "../BasicForm";
import { HttpMethod } from "../FormTypes";

type Props = {
  id: string;
};

const FIELDS = {
  email: {
    ...api.userSignUp.post.email,
    id: "FORMS_SIGN_UP_FIELDS_EMAIL"
  },
  login: {
    ...api.userSignUp.post.login,
    id: "FORMS_SIGN_UP_FIELDS_IDENTIFIER"
  },
  password: {
    ...api.userSignUp.post.password,
    id: "FORMS_SIGN_UP_FIELDS_PASSWORD"
  },
  confirm: {
    ...api.userSignUp.post.confirm,
    id: "FORMS_SIGN_UP_FIELDS_CONFIRM"
  }
};

const loginInputProperties = { required: FIELDS.login.required };
const passwordInputProperties = { required: FIELDS.password.required };
const confirmInputProperties = { required: FIELDS.confirm.required };
const emailInputProperties = { required: FIELDS.email.required };

function SignUp({ id }: Props) {
  const { t } = useTranslation();

  return (
    <BasicForm
      id={id}
      method={HttpMethod.POST}
      action={`/api${api.userSignUp.path}`}
      fields={Object.values(FIELDS)}
      submitTitle="FORMS.SIGN_UP.FIELDS.SUBMIT.LABEL"
    >
      <fieldset>
        <legend>{t("FORMS.SIGN_UP.TITLE")}</legend>

        <BasicInput
          type="email"
          name={FIELDS.email.name}
          id={FIELDS.email.id}
          inputProperties={emailInputProperties}
          label={t("FORMS.SIGN_UP.FIELDS.EMAIL.LABEL")}
        />

        <BasicInput
          type="text"
          name={FIELDS.login.name}
          id={FIELDS.login.id}
          inputProperties={loginInputProperties}
          label={t("FORMS.SIGN_UP.FIELDS.IDENTIFIER.LABEL")}
        />

        <BasicInput
          type="password"
          name={FIELDS.password.name}
          id={FIELDS.password.id}
          inputProperties={passwordInputProperties}
          label={t("FORMS.SIGN_UP.FIELDS.PASSWORD.LABEL")}
        />

        <BasicInput
          type="password"
          name={FIELDS.confirm.name}
          id={FIELDS.confirm.id}
          inputProperties={confirmInputProperties}
          label={t("FORMS.SIGN_UP.FIELDS.CONFIRM.LABEL")}
        />
      </fieldset>
    </BasicForm>
  );
}

export default SignUp;
